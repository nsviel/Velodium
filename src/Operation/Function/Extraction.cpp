#include "Extraction.h"

#include "../Node_operation.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"


//Constructor / destructor
Extraction::Extraction(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();

  this->sceneManager = node_engine->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();
  this->loaderManager = node_load->get_loadManager();
  this->objectManager = node_engine->get_objectManager();

  this->list_part = new list<subpart*>;
  this->highlightON = false;
  this->sliceON = false;
  this->ID_cloud = 0;
  this->ID_part = 0;

  //---------------------------
}
Extraction::~Extraction(){}

//Extract / Cutting function
void Extraction::fct_extractCloud(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  Subset* subset_init = sceneManager->get_subset_selected_init();
  //---------------------------

  //New cloud
  Cloud* cloud_out = new Cloud();
  Subset* subset_out = new Subset();

  //Parameters
  vector<vec3>& XYZ = subset->xyz;
  const vector<vec4>& RGB = subset_init->RGB;
  vector<vec3>& N = subset->N;
  vector<float>& Is = subset->I;
  vec3& max = subset->max;
  vec3& min = subset->min;
  cloud_out->format = ".pts";
  subset_out->name = subset->name + "_" + "part" + to_string(ID_cloud);
  ID_cloud++;

  //Take values between sliceMin and sliceMax
  for (int i=0; i<XYZ.size(); i++)
    if(XYZ[i].x >= min.x && XYZ[i].x <= max.x &&
      XYZ[i].y >= min.y && XYZ[i].y <= max.y &&
      XYZ[i].z >= min.z && XYZ[i].z <= max.z){
      //Location
      if(sliceON){
        subset_out->xyz.push_back(vec3(XYZ[i].x, XYZ[i].y,  min.z));
      }
      else{
        subset_out->xyz.push_back(vec3(XYZ[i].x, XYZ[i].y,  XYZ[i].z));
      }
      //Color
      if(subset->has_color){
        subset_out->RGB.push_back(vec4(RGB[i].x, RGB[i].y, RGB[i].z, RGB[i].w));
        subset_out->has_color = true;
      }
      //Normal
      if(subset->N.size() != 0){
        subset_out->N.push_back(vec3(N[i].x, N[i].y, N[i].z));
      }
      //Intensity
      if(subset->I.size() != 0){
        subset_out->I.push_back(Is[i]);
      }
    }

  //---------------------------
  if(subset_out->xyz.size() != 0){
    //Create slice if any points
    cloud_out->subset.push_back(subset_out);
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points detected"<<endl;
  }
}
void Extraction::fct_extractSelected(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  Subset* subset_init = sceneManager->get_subset_selected_init();
  //---------------------------

  //New cloud
  Cloud* cloud_out = new Cloud();
  Subset* subset_out = new Subset();

  //Parameters
  vector<vec3>& XYZ = subset->xyz;
  const vector<vec4>& RGB = subset_init->RGB;
  vector<vec3>& N = subset->N;
  vector<float>& Is = subset->I;

  cloud_out->format = ".pts";
  subset_out->name = subset->name + "_" + "part" + to_string(ID_cloud);
  ID_cloud++;
  vector<int>& idx = subset->selected;
  //---------------------------

  for(int i=0; i<idx.size(); i++){
    subset_out->xyz.push_back(vec3(XYZ[idx[i]].x, XYZ[idx[i]].y,  XYZ[idx[i]].z));

    //Color
    if(subset->has_color){
      subset_out->RGB.push_back(vec4(RGB[idx[i]].x, RGB[idx[i]].y, RGB[idx[i]].z, RGB[idx[i]].w));
      subset_out->has_color = true;
    }

    //Normal
    if(subset->N.size() != 0){
      subset_out->N.push_back(vec3(N[idx[i]].x, N[idx[i]].y, N[idx[i]].z));
    }

    //Intensity
    if(subset->I.size() != 0){
      subset_out->I.push_back(Is[idx[i]]);
    }
  }

  //---------------------------
  idx.clear();
  if(subset_out->xyz.size() != 0){
    cloud_out->subset.push_back(subset_out);
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points selected"<<endl;
  }
}
void Extraction::fct_cutCloud(Subset* subset){
  vector<vec3>& XYZ = subset->xyz;
  vec3& max = subset->max;
  vec3& min = subset->min;
  vector<int> idx;
  //---------------------------

  //Take values between sliceMin and sliceMax
  for(int i=0; i<XYZ.size(); i++){
    if(XYZ[i].x < min.x || XYZ[i].x > max.x ||
      XYZ[i].y < min.y || XYZ[i].y > max.y ||
      XYZ[i].z < min.z || XYZ[i].z > max.z){
      idx.push_back(i);
    }
  }

  //Supress non selected points
  attribManager->make_supressPoints(subset, idx);

  //---------------------------
}
void Extraction::fct_cutCloud_all(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  Cloud* cloud_selected = sceneManager->get_selected_cloud();
  vec3& max = cloud_selected->max;
  vec3& min = cloud_selected->min;
  //---------------------------

  for(int i=0;i<list_cloud->size();i++){
    //Select ieme Point Cloud
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    vector<vec3>& XYZ = subset->xyz;
    vector<int> idx;

    //Take values between sliceMin and sliceMax
    for (int i=0; i<XYZ.size(); i++){
      if(XYZ[i].x < min.x || XYZ[i].x > max.x ||
        XYZ[i].y < min.y || XYZ[i].y > max.y ||
        XYZ[i].z < min.z || XYZ[i].z > max.z){
        idx.push_back(i);
      }
    }

    //Supress non selected points
    attribManager->make_supressPoints(subset, idx);
  }

  //---------------------------
}
void Extraction::supress_selectedpart(subpart* part){
  //---------------------------

  if(list_part->size() != 0){
    int ID = part->ID;

    list<subpart*>::iterator it = next(list_part->begin(), ID);
    list_part->erase(it);
  }

  //---------------------------
}
void Extraction::fct_selectPart(Subset* subset, vec3 mina, vec3 maxa){
  subpart* part = new subpart;
  //---------------------------

  vec3 max = subset->max;
  vec3 min = subset->min;

  part->ID = ID_part;
  part->name = to_string(ID_part);
  part->namePC = subset->name;
  part->minloc = min;
  part->maxloc = max;

  //---------------------------
  list_part->push_back(part);
  ID_part++;
}

//Merging function
void Extraction::fct_merging_list(vector<Cloud*> list_part){
  //---------------------------

  //New cloud
  Cloud* cloud_out = new Cloud();
  Subset* subset_out = new Subset();

  for(int i=0; i<list_part.size()-1; i++){
    Cloud* part_1 = list_part[i];
    Cloud* part_2 = list_part[i+1];

    Subset* subset_1 = sceneManager->get_subset(part_1, 0);
    Subset* subset_2 = sceneManager->get_subset(part_2, 0);

    //Location
    vector<vec3>& XYZ_1 = subset_1->xyz;
    vector<vec3>& XYZ_2 = subset_2->xyz;
    vector<vec3>& XYZ_out = subset_out->xyz;

    XYZ_out.insert( XYZ_out.end(), XYZ_1.begin(), XYZ_1.end());
    XYZ_out.insert( XYZ_out.end(), XYZ_2.begin(), XYZ_2.end());

    //Color
    if(subset_1->has_color && subset_2->has_color){
      vector<vec4>& RGB_1 = subset_1->RGB;
      vector<vec4>& RGB_2 = subset_2->RGB;
      vector<vec4>& RGB_out = subset_out->RGB;

      RGB_out.insert( RGB_out.end(), RGB_1.begin(), RGB_1.end());
      RGB_out.insert( RGB_out.end(), RGB_2.begin(), RGB_2.end());

      subset_out->has_color = true;
    }
    //Normal
    if(subset_1->N.size() != 0 && subset_2->N.size() != 0){
      vector<vec3>& N_1 = subset_1->N;
      vector<vec3>& N_2 = subset_2->N;
      vector<vec3>& N_out = subset_out->N;

      N_out.insert( N_out.end(), N_1.begin(), N_1.end());
      N_out.insert( N_out.end(), N_2.begin(), N_2.end());
    }
    //Intensity
    if(subset_1->I.size() != 0 && subset_2->I.size() != 0){
      vector<float>& Is_1 = subset_1->I;
      vector<float>& Is_2 = subset_2->I;
      vector<float>& Is_out = subset_out->I;

      Is_out.insert( Is_out.end(), Is_1.begin(), Is_1.end());
      Is_out.insert( Is_out.end(), Is_2.begin(), Is_2.end());
    }
  }

  //Infos
  subset_out->name = "Merged cloud";
  cloud_out->format = ".pts";
  cloud_out->nb_point = subset_out->xyz.size();

  //---------------------------
  if(cloud_out->nb_point > 0){
    cloud_out->subset.push_back(subset_out);
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points available"<<endl;
  }
}
void Extraction::fct_merging_newCloud(Cloud* cloud_1, Cloud* cloud_2){
  //---------------------------

  //New cloud
  Cloud* cloud_out = new Cloud();
  Subset* subset_out = new Subset();

  Subset* subset_1 = sceneManager->get_subset(cloud_1, 0);
  Subset* subset_2 = sceneManager->get_subset(cloud_2, 0);

  //Location
  vector<vec3>& XYZ_1 = subset_1->xyz;
  vector<vec3>& XYZ_2 = subset_2->xyz;
  vector<vec3>& XYZ_out = subset_out->xyz;
  XYZ_out.insert( XYZ_out.end(), XYZ_1.begin(), XYZ_1.end());
  XYZ_out.insert( XYZ_out.end(), XYZ_2.begin(), XYZ_2.end());

  //Color
  if(subset_1->has_color && subset_2->has_color){
    vector<vec4>& RGB_1 = subset_1->RGB;
    vector<vec4>& RGB_2 = subset_2->RGB;
    vector<vec4>& RGB_out = subset_out->RGB;

    RGB_out.insert( RGB_out.end(), RGB_1.begin(), RGB_1.end());
    RGB_out.insert( RGB_out.end(), RGB_2.begin(), RGB_2.end());

    subset_out->has_color = true;
  }
  //Normal
  if(subset_1->N.size() != 0 && subset_2->N.size() != 0){
    vector<vec3>& N_1 = subset_1->N;
    vector<vec3>& N_2 = subset_2->N;
    vector<vec3>& N_out = subset_out->N;

    N_out.insert( N_out.end(), N_1.begin(), N_1.end());
    N_out.insert( N_out.end(), N_2.begin(), N_2.end());
  }
  //Intensity
  if(subset_1->I.size() != 0 && subset_2->I.size() != 0){
    vector<float>& Is_1 = subset_1->I;
    vector<float>& Is_2 = subset_2->I;
    vector<float>& Is_out = subset_out->I;

    Is_out.insert( Is_out.end(), Is_1.begin(), Is_1.end());
    Is_out.insert( Is_out.end(), Is_2.begin(), Is_2.end());
  }

  //Infos
  subset_out->name = "Merging_" + subset_1->name + "_" + subset_2->name;
  cloud_out->format = ".pts";
  cloud_out->nb_point = subset_out->xyz.size();

  //Create slice if any points
  if(cloud_out->nb_point != 0){
    cloud_out->subset.push_back(subset_out);
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points available"<<endl;
  }

  //---------------------------
}
void Extraction::fct_merging_addCloud(Cloud* cloud_1, Cloud* cloud_2){
  //---------------------------

  Subset* subset_1 = sceneManager->get_subset(cloud_1, 0);
  Subset* subset_1_init = sceneManager->get_subset_init(cloud_1, 0);
  Subset* subset_2 = sceneManager->get_subset(cloud_2, 0);

  //Location
  vector<vec3>& XYZ_1 = subset_1->xyz;
  vector<vec3>& XYZ_2 = subset_2->xyz;
  XYZ_1.insert( XYZ_1.end(), XYZ_2.begin(), XYZ_2.end());
  subset_1_init->xyz = XYZ_1;
  cloud_1->nb_point = XYZ_1.size();

  //Color
  if(subset_1->has_color && subset_2->has_color){
    vector<vec4>& RGB_1 = subset_1->RGB;
    vector<vec4>& RGB_2 = subset_2->RGB;
    RGB_1.insert( RGB_1.end(), RGB_2.begin(), RGB_2.end());
    subset_1_init->RGB = RGB_1;
  }
  //Normal
  if(subset_1->N.size() != 0 && subset_2->N.size() != 0){
    vector<vec3>& N_1 = subset_1->N;
    vector<vec3>& N_2 = subset_2->N;
    N_1.insert( N_1.end(), N_2.begin(), N_2.end());
    subset_1_init->N = N_1;
  }
  //Intensity
  if(subset_1->I.size() != 0 && subset_2->I.size() != 0){
    vector<float>& Is_1 = subset_1->I;
    vector<float>& Is_2 = subset_2->I;
    Is_1.insert( Is_1.end(), Is_2.begin(), Is_2.end());
    subset_1_init->I = Is_1;
  }

  //---------------------------
}

//Selection function
void Extraction::fct_highlighting(Subset* subset, Subset* subset_init){
  vec3 max = subset->max;
  vec3 min = subset->min;
  vector<vec3>& pos = subset->xyz;
  vector<vec4>& color = subset->RGB;
  vector<vec4>& RGB = subset_init->RGB;
  //---------------------------

  if(highlightON == true){
    for(int i=0; i<pos.size(); i++){
      if(pos[i].x >= min.x &&
        pos[i].y >= min.y &&
        pos[i].z >= min.z &&
        pos[i].x <= max.x &&
        pos[i].y <= max.y &&
        pos[i].z <= max.z){
        //Qualify color according to previous unlighting color
        color[i] = vec4(1,color[i].y,color[i].z,1);
      }
      else{
        //Restaure original color
        color[i] = RGB[i];
      }
    }
  }
  else{
    subset->RGB = RGB;
  }

  //---------------------------
  sceneManager->update_subset_color(subset);
}
void Extraction::set_AABB_min(vec3 min_in){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  Subset* subset_init = sceneManager->get_subset_selected_init();
  //---------------------------

  vec3 max_old = subset->max;
  vec3 min_old = subset->min;

  //Get Z extremums
  sceneManager->update_cloud_MinMax(cloud);
  vec3 min = subset->min;
  vec3 max = subset->max;
  vec3 diff = max - min;
  vec3 min_out;

  for(int i=0; i<3; i++){
    if(min_in[i] > 100) min_in[i] = 100;
    if(min_in[i] <= 0) diff[i] = 0;
    else diff[i] = diff[i] * min_in[i]/100;

    min_out[i] = min[i] + diff[i];
    if(min_out[i] > max_old[i]) min_out[i] = max_old[i];
  }

  subset->max = max_old;
  subset->min = min_old;
  subset->min = min_out;

  //---------------------------
  this->fct_highlighting(subset, subset_init);
  objectManager->update_glyph_cloud(cloud);
}
void Extraction::set_AABB_max(vec3 max_in){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  Subset* subset_init = sceneManager->get_subset_selected_init();
  //---------------------------

  vec3 max_old = subset->max;
  vec3 min_old = subset->min;

  //Get Z extremums
  sceneManager->update_cloud_MinMax(cloud);
  vec3 min = subset->min;
  vec3 max = subset->max;
  vec3 diff = max - min;
  vec3 max_out;

  for(int i=0; i<3; i++){
    if(max_in[i] > 100) max_in[i] = 100;
    if(max_in[i] <= 0) diff[i] = 0;
    else diff[i] = diff[i] * max_in[i]/100;

    max_out[i] = min[i] + diff[i];
    if(max_out[i] < min_old[i]) max_out[i] = min_old[i];
  }

  subset->max = max_old;
  subset->min = min_old;
  subset->max = max_out;

  //---------------------------
  this->fct_highlighting(subset, subset_init);
}
