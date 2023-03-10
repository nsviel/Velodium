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
  list<Collection*>* list_collection = sceneManager->get_list_collection();
  //---------------------------

  for(int i=0; i<list_collection->size(); i++){
    Collection* cloud = *next(list_collection->begin(),i);

    cloud->is_heatmap = is_heatmap;
    this->make_cloud_heatmap(cloud);
  }

  //---------------------------
}
void Heatmap::make_cloud_heatmap(Collection* cloud){
  //---------------------------

  //Apply or reverse heatmap for cloud
  for(int i=0; i<cloud->list_obj.size(); i++){
    Cloud* subset = (Cloud*)cloud->get_obj(i);
    Cloud* subset_buf = (Cloud*)cloud->get_obj_buffer(i);
    Cloud* subset_ini = (Cloud*)cloud->get_obj_init(i);

    //Apply heatmap
    if(cloud->is_heatmap == false){
      subset_buf->rgb = subset->rgb;
      this->make_subset_heatmap(subset);
      cloud->is_heatmap = true;
    }
    //Reverse heatmap
    else{
      //this->heatmap_unset(subset);
      subset->rgb = subset_buf->rgb;
      cloud->is_heatmap = false;
    }
  }

  //---------------------------
  //sceneManager->update_buffer_color(cloud);
}
void Heatmap::make_subset_heatmap(Cloud* subset){
  //---------------------------

  switch(heatmap_mode){
    case 0:{// height
      this->mode_height(subset);
      break;
    }
    case 1:{// intensity
      this->mode_intensity(subset);
      break;
    }
    case 2:{// distance
      this->mode_distance(subset);
      break;
    }
    case 3:{// incidence angle cosine
      this->mode_cosIt(subset);
      break;
    }
    case 4:{// incidence angle
      this->mode_It(subset);
      break;
    }
  }

  //---------------------------
  gpuManager->update_buffer_color(subset);
}

//Specific mode functions
void Heatmap::mode_height(Cloud* subset){
  //---------------------------

  vector<float> z_vec = attribManager->get_z_vector(subset->xyz);

  //Check for preventing too much near range
  if(range_height.x + 1 > range_height.y){
    range_height.x = range_height.y - 1;
  }

  //fct_normalize resulting color vector
  vector<float> z_vec_norm = fct_normalize(z_vec, range_height);
  z_vec_norm = fct_standardize(z_vec_norm, -1);
  vector<float>& color_vec = z_vec_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_intensity(Cloud* subset){
  if(subset->I.size() != 0){
    //---------------------------

    vector<float>& Is = subset->I;
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
    this->heatmap_set(subset, color_vec);
  }
}
void Heatmap::mode_distance(Cloud* subset){
  vector<float>& dist = subset->R;
  //---------------------------

  if(dist.size() == 0){
    attribManager->compute_subset_distance(subset);
  }

  vector<float> dist_norm = fct_normalize(dist);
  vector<float>& color_vec = dist_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_cosIt(Cloud* subset){
  vector<float>& color_vec = subset->cosIt;
  //---------------------------

  if(color_vec.size() == 0){
    attribManager->compute_subset_cosIt(subset);
  }

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_It(Cloud* subset){
  vector<float>& It = subset->It;
  //---------------------------

  if(It.size() == 0){
    attribManager->compute_subset_cosIt(subset);
  }

  vector<float> It_norm = fct_normalize(It);
  vector<float>& color_vec = It_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}

//Processing functions
void Heatmap::heatmap_set(Cloud* subset, vector<float>& v_in){
  vector<vec4>& RGB = subset->rgb;
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
void Heatmap::heatmap_unset(Cloud* subset){
  vector<vec4>& RGB = subset->rgb;
  vector<float>& Is = subset->I;
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
      RGB[i] = subset->unicolor;
    }
  }


  //---------------------------
}
