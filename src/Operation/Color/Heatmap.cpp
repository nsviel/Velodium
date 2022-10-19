#include "Heatmap.h"

#include "Colormap.h"

#include "../Node_operation.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Specific/fct_maths.h"

/* heatmap_mode
 * 0 = height
 * 1 = intensity
 * 2 = distance
 * 3 = incidence angle cosine
 * 4 = incidence angle
*/


//Constructor / destructor
Heatmap::Heatmap(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->colormapManager = new Colormap();
  this->sceneManager = node_engine->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();

  this->heatmap_mode = 1;
  this->is_normalization = true;
  this->range_norm = vec2(0.0f, 1.0f);
  this->range_height = vec2(-2.0f, 2.0f);
  this->range_intensity = vec2(0.0f, 1.0f);

  //---------------------------
}
Heatmap::~Heatmap(){}

//HMI functions
void Heatmap::make_heatmap_all(bool is_heatmap){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    cloud->heatmap = is_heatmap;
    this->make_cloud_heatmap(cloud);
  }

  //---------------------------
}
void Heatmap::make_cloud_heatmap(Cloud* cloud){
  //---------------------------

  //Apply or reverse heatmap for cloud
  for(int i=0; i<cloud->subset.size(); i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    Subset* subset_buf = sceneManager->get_subset_buffer(cloud, i);
    Subset* subset_ini = sceneManager->get_subset_init(cloud, i);

    //Apply heatmap
    if(cloud->heatmap == false){
      subset_buf->RGB = subset->RGB;
      this->make_subset_heatmap(subset);
      cloud->heatmap = true;
    }
    //Reverse heatmap
    else{
      //this->heatmap_unset(subset);
      subset->RGB = subset_buf->RGB;
      cloud->heatmap = false;
    }
  }

  //---------------------------
  sceneManager->update_cloud_color(cloud);
}
void Heatmap::make_subset_heatmap(Subset* subset){
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
  sceneManager->update_subset_color(subset);
}

//Specific mode functions
void Heatmap::mode_height(Subset* subset){
  //---------------------------

  vector<float> z_vec = attribManager->get_z_vector(subset->xyz);

  //Check for preventing too much near range
  if(range_height.x + 1 > range_height.y){
    range_height.x = range_height.y - 1;
  }

  //fct_normalize resulting color vector
  vector<float> z_vec_norm = fct_normalize(z_vec, range_height);
  vector<float>& color_vec = z_vec_norm;

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_intensity(Subset* subset){
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
void Heatmap::mode_distance(Subset* subset){
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
void Heatmap::mode_cosIt(Subset* subset){
  vector<float>& color_vec = subset->cosIt;
  //---------------------------

  if(color_vec.size() == 0){
    attribManager->compute_subset_cosIt(subset);
  }

  //---------------------------
  this->heatmap_set(subset, color_vec);
}
void Heatmap::mode_It(Subset* subset){
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
void Heatmap::heatmap_set(Subset* subset, vector<float>& v_in){
  vector<vec4>& RGB = subset->RGB;
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
void Heatmap::heatmap_unset(Subset* subset){
  vector<vec4>& RGB = subset->RGB;
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
