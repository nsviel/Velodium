#include "Heatmap.h"

#include "Colormap.h"

#include "../Node_operation.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/GPU/GPU_data.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Specific/Function/fct_math.h"

/* heatmap_mode
 * 0 = height
 * 1 = intensity
 * 2 = distance
 * 3 = incidence angle cosine
 * 4 = incidence angle
*/


//Constructor / destructor
Heatmap::Heatmap(){
  //---------------------------

  this->colormapManager = new Colormap();
  this->sceneManager = new Scene();
  this->attribManager = new Attribut();
  this->gpuManager = new GPU_data();

  this->heatmap_mode = 1;
  this->is_normalization = true;
  this->range_norm = vec2(0.0f, 1.0f);
  this->range_height = vec2(-2.0f, 2.0f);
  this->range_intensity = vec2(0.0f, 1.0f);

  //---------------------------
}
Heatmap::~Heatmap(){
  //---------------------------

  delete colormapManager;
  delete sceneManager;
  delete attribManager;
  delete gpuManager;

  //---------------------------
}

//HMI functions
void Heatmap::make_heatmap_all(bool is_heatmap){
  list<Collection*>* list_collection = sceneManager->get_list_col_object();
  //---------------------------

  for(int i=0; i<list_collection->size(); i++){
    Collection* collection = *next(list_collection->begin(),i);

    collection->is_heatmap = is_heatmap;
    this->make_col_heatmap(collection);
  }

  //---------------------------
}
void Heatmap::make_col_heatmap(Collection* collection){
  //---------------------------

  //Apply or reverse heatmap for cloud
  for(int i=0; i<collection->list_obj.size(); i++){
    Cloud* cloud = (Cloud*)collection->get_obj(i);
    Cloud* subset_buf = (Cloud*)collection->get_obj_buffer(i);
    Cloud* subset_ini = (Cloud*)collection->get_obj_init(i);

    //Apply heatmap
    if(collection->is_heatmap == false){
      subset_buf->rgb = cloud->rgb;
      this->make_cloud_heatmap(cloud);
      collection->is_heatmap = true;
    }
    //Reverse heatmap
    else{
      //this->heatmap_unset(cloud);
      cloud->rgb = subset_buf->rgb;
      collection->is_heatmap = false;
    }
  }

  //---------------------------
  //sceneManager->update_buffer_color(cloud);
}
void Heatmap::make_cloud_heatmap(Cloud* cloud){
  //---------------------------

  switch(heatmap_mode){
    case 0:{// height
      this->mode_height(cloud);
      break;
    }
    case 1:{// intensity
      this->mode_intensity(cloud);
      break;
    }
    case 2:{// distance
      this->mode_distance(cloud);
      break;
    }
    case 3:{// incidence angle cosine
      this->mode_cosIt(cloud);
      break;
    }
    case 4:{// incidence angle
      this->mode_It(cloud);
      break;
    }
  }

  //---------------------------
  gpuManager->update_buffer_color(cloud);
}

//Specific mode functions
void Heatmap::mode_height(Cloud* cloud){
  //---------------------------

  vector<float> z_vec = attribManager->get_z_vector(cloud->xyz);

  //Check for preventing too much near range
  if(range_height[0] + 1 > range_height[1]){
    range_height[0] = range_height.y - 2;
  }

  //fct_normalize resulting color vector
  vector<float> z_vec_norm = fct_normalize(z_vec, range_height);
  //z_vec_norm = fct_standardize(z_vec_norm, -1);
  vector<float>& color_vec = z_vec_norm;

  //---------------------------
  this->heatmap_set(cloud, color_vec);
}
void Heatmap::mode_intensity(Cloud* cloud){
  if(cloud->I.size() != 0){
    //---------------------------

    vector<float>& Is = cloud->I;
    vector<float> color_vec;

    for(int i=0; i<Is.size(); i++){
      //If point i intensity is in range
      if(Is[i] >= range_intensity.x && Is[i] <= range_intensity.y){
        color_vec.push_back(Is[i]);
      }
      //If point i intensity is out of range
      else{
        color_vec.push_back(-1);
      }
    }

    //---------------------------
    this->heatmap_set(cloud, color_vec);
  }
}
void Heatmap::mode_distance(Cloud* cloud){
  vector<float>& dist = cloud->R;
  //---------------------------

  if(dist.size() == 0){
    attribManager->compute_subset_distance(cloud);
  }

  vector<float> dist_norm = fct_normalize(dist);
  vector<float>& color_vec = dist_norm;

  //---------------------------
  this->heatmap_set(cloud, color_vec);
}
void Heatmap::mode_cosIt(Cloud* cloud){
  vector<float>& color_vec = cloud->cosIt;
  //---------------------------

  if(color_vec.size() == 0){
    attribManager->compute_subset_cosIt(cloud);
  }

  //---------------------------
  this->heatmap_set(cloud, color_vec);
}
void Heatmap::mode_It(Cloud* cloud){
  vector<float>& It = cloud->It;
  //---------------------------

  if(It.size() == 0){
    attribManager->compute_subset_cosIt(cloud);
  }

  vector<float> It_norm = fct_normalize(It);
  vector<float>& color_vec = It_norm;

  //---------------------------
  this->heatmap_set(cloud, color_vec);
}

//Processing functions
void Heatmap::heatmap_set(Cloud* cloud, vector<float>& v_in){
  vector<vec4>& RGB = cloud->rgb;
  //---------------------------

  //Normalization of the input vector
  vector<float> v_norm;
  if(is_normalization){
    v_norm = fct_normalize(v_in, -1);
  }else{
    v_norm = v_in;
  }

  //Compute heatmap from input vector
  for(int i=0; i<RGB.size(); i++){
    if(v_in[i] != -1 && isnan(v_norm[i]) == false){
      vector<vec3>* colormap = colormapManager->get_colormap_selected();

      float value = v_norm[i] * (colormap->size()-1);        // Will multiply value by 3.
      float idx1  = floor(value);                  // Our desired color will be after this index.
      float idx2  = idx1 + 1;                        // ... and before this index (inclusive).
      float fractBetween = value - float(idx1);    // Distance between the two indexes (0-1).

      float red   = ((*colormap)[idx2][0] - (*colormap)[idx1][0]) * fractBetween + (*colormap)[idx1][0];
      float green = ((*colormap)[idx2][1] - (*colormap)[idx1][1]) * fractBetween + (*colormap)[idx1][1];
      float blue  = ((*colormap)[idx2][2] - (*colormap)[idx1][2]) * fractBetween + (*colormap)[idx1][2];

      RGB[i] = vec4(red, green, blue, 1.0f);
    }
    else{
      RGB[i] = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }

  //---------------------------
}
void Heatmap::heatmap_unset(Cloud* cloud){
  vector<vec4>& RGB = cloud->rgb;
  vector<float>& Is = cloud->I;
  //---------------------------

  //If intensity, reapply color
  if(Is.size() != 0){
    for(int i=0; i<Is.size(); i++){
      RGB[i] = vec4(Is[i], Is[i], Is[i],1);
    }
  }
  //else reapply random color
  else{
    for(int i=0; i<RGB.size(); i++){
      RGB[i] = cloud->unicolor;
    }
  }


  //---------------------------
}
