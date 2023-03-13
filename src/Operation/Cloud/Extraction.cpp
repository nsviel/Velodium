#include "Extraction.h"

#include "../Node_operation.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Scene/Node_scene.h"
#include "../../Scene/Data/Scene.h"
#include "../../Scene/Glyph/Object.h"
#include "../../Load/Node_load.h"
#include "../../Load/Processing/Loader.h"


//Constructor / destructor
Extraction::Extraction(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();
  Node_load* node_load = node_engine->get_node_load();
  Node_scene* node_scene = node_engine->get_node_scene();

  this->sceneManager = node_scene->get_sceneManager();
  this->attribManager = node_ope->get_attribManager();
  this->loaderManager = node_load->get_loaderManager();
  this->objectManager = node_scene->get_objectManager();

  this->list_part = new list<subpart*>;
  this->highlightON = false;
  this->sliceON = false;
  this->ID_cloud = 0;
  this->ID_part = 0;

  //---------------------------
}
Extraction::~Extraction(){}

//Extract / Cutting function
void Extraction::fct_extractCloud(Collection* collection){
  Cloud* cloud = (Cloud*)collection->selected_obj;
  Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
  //---------------------------

  //New cloud
  Collection* col_out = new Collection();
  Cloud* cloud_out = new Cloud();

  //Parameters
  vector<vec3>& XYZ = cloud->xyz;
  const vector<vec4>& RGB = list_obj_init->rgb;
  vector<vec3>& N = cloud->Nxyz;
  vector<float>& Is = cloud->I;
  vec3& max = cloud->max;
  vec3& min = cloud->min;
  col_out->file_format = ".pts";
  cloud_out->name = cloud->name + "_" + "part" + to_string(ID_cloud);
  ID_cloud++;

  //Take values between sliceMin and sliceMax
  for (int i=0; i<XYZ.size(); i++)
    if(XYZ[i].x >= min.x && XYZ[i].x <= max.x &&
      XYZ[i].y >= min.y && XYZ[i].y <= max.y &&
      XYZ[i].z >= min.z && XYZ[i].z <= max.z){
      //Location
      if(sliceON){
        cloud_out->xyz.push_back(vec3(XYZ[i].x, XYZ[i].y,  min.z));
      }
      else{
        cloud_out->xyz.push_back(vec3(XYZ[i].x, XYZ[i].y,  XYZ[i].z));
      }
      //Color
      if(cloud->has_color){
        cloud_out->rgb.push_back(vec4(RGB[i].x, RGB[i].y, RGB[i].z, RGB[i].w));
        cloud_out->has_color = true;
      }
      //Normal
      if(cloud->Nxyz.size() != 0){
        cloud_out->Nxyz.push_back(vec3(N[i].x, N[i].y, N[i].z));
      }
      //Intensity
      if(cloud->I.size() != 0){
        cloud_out->I.push_back(Is[i]);
      }
    }

  //---------------------------
  if(cloud_out->xyz.size() != 0){
    //Create slice if any points
    col_out->list_obj.push_back(cloud_out);
    loaderManager->load_cloud_creation(col_out);
  }
  else{
    cout<<"No points detected"<<endl;
  }
}
void Extraction::fct_extractSelected(Collection* collection){
  Cloud* cloud = (Cloud*)collection->selected_obj;
  Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
  //---------------------------

  //New cloud
  Collection* col_out = new Collection();
  Cloud* cloud_out = new Cloud();

  //Parameters
  vector<vec3>& XYZ = cloud->xyz;
  const vector<vec4>& RGB = list_obj_init->rgb;
  vector<vec3>& N = cloud->Nxyz;
  vector<float>& Is = cloud->I;

  col_out->file_format = ".pts";
  cloud_out->name = cloud->name + "_" + "part" + to_string(ID_cloud);
  ID_cloud++;
  vector<int>& idx = cloud->selected;
  //---------------------------

  for(int i=0; i<idx.size(); i++){
    cloud_out->xyz.push_back(vec3(XYZ[idx[i]].x, XYZ[idx[i]].y,  XYZ[idx[i]].z));

    //Color
    if(cloud->has_color){
      cloud_out->rgb.push_back(vec4(RGB[idx[i]].x, RGB[idx[i]].y, RGB[idx[i]].z, RGB[idx[i]].w));
      cloud_out->has_color = true;
    }

    //Normal
    if(cloud->Nxyz.size() != 0){
      cloud_out->Nxyz.push_back(vec3(N[idx[i]].x, N[idx[i]].y, N[idx[i]].z));
    }

    //Intensity
    if(cloud->I.size() != 0){
      cloud_out->I.push_back(Is[idx[i]]);
    }
  }

  //---------------------------
  idx.clear();
  if(cloud_out->xyz.size() != 0){
    col_out->list_obj.push_back(cloud_out);
    loaderManager->load_cloud_creation(col_out);
  }
  else{
    cout<<"No points selected"<<endl;
  }
}
void Extraction::fct_cutCloud(Cloud* cloud){
  vector<vec3>& XYZ = cloud->xyz;
  vec3& max = cloud->max;
  vec3& min = cloud->min;
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
  attribManager->make_supressPoints(cloud, idx);

  //---------------------------
}
void Extraction::fct_cutCloud_all(){
  /*list<Collection*>* list_collection = sceneManager->get_list_col_object();
  Collection* collection_selected = sceneManager->get_selected_collection();
  vec3& max = collection_selected->max;
  vec3& min = collection_selected->min;
  //---------------------------

  for(int i=0;i<list_collection->size();i++){
    //Select ieme Point Cloud
    Collection* collection = *next(list_collection->begin(),i);
    Cloud* cloud = (Cloud*)collection->selected_obj;

    vector<vec3>& XYZ = cloud->xyz;
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
    attribManager->make_supressPoints(cloud, idx);
  }

  //---------------------------*/
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
void Extraction::fct_selectPart(Cloud* cloud, vec3 mina, vec3 maxa){
  subpart* part = new subpart;
  //---------------------------

  vec3 max = cloud->max;
  vec3 min = cloud->min;

  part->ID = ID_part;
  part->name = to_string(ID_part);
  part->namePC = cloud->name;
  part->minloc = min;
  part->maxloc = max;

  //---------------------------
  list_part->push_back(part);
  ID_part++;
}

//Merging function
void Extraction::fct_merging_list(vector<Collection*> list_part){
  //---------------------------

  //New cloud
  Collection* col_out = new Collection();
  Cloud* cloud_out = new Cloud();

  for(int i=0; i<list_part.size()-1; i++){
    Collection* part_1 = list_part[i];
    Collection* part_2 = list_part[i+1];

    Cloud* subset_1 = (Cloud*)part_1->get_obj(0);
    Cloud* subset_2 = (Cloud*)part_2->get_obj(0);

    //Location
    vector<vec3>& XYZ_1 = subset_1->xyz;
    vector<vec3>& XYZ_2 = subset_2->xyz;
    vector<vec3>& XYZ_out = cloud_out->xyz;

    XYZ_out.insert( XYZ_out.end(), XYZ_1.begin(), XYZ_1.end());
    XYZ_out.insert( XYZ_out.end(), XYZ_2.begin(), XYZ_2.end());

    //Color
    if(subset_1->has_color && subset_2->has_color){
      vector<vec4>& RGB_1 = subset_1->rgb;
      vector<vec4>& RGB_2 = subset_2->rgb;
      vector<vec4>& RGB_out = cloud_out->rgb;

      RGB_out.insert( RGB_out.end(), RGB_1.begin(), RGB_1.end());
      RGB_out.insert( RGB_out.end(), RGB_2.begin(), RGB_2.end());

      cloud_out->has_color = true;
    }
    //Normal
    if(subset_1->Nxyz.size() != 0 && subset_2->Nxyz.size() != 0){
      vector<vec3>& N_1 = subset_1->Nxyz;
      vector<vec3>& N_2 = subset_2->Nxyz;
      vector<vec3>& N_out = cloud_out->Nxyz;

      N_out.insert( N_out.end(), N_1.begin(), N_1.end());
      N_out.insert( N_out.end(), N_2.begin(), N_2.end());
    }
    //Intensity
    if(subset_1->I.size() != 0 && subset_2->I.size() != 0){
      vector<float>& Is_1 = subset_1->I;
      vector<float>& Is_2 = subset_2->I;
      vector<float>& Is_out = cloud_out->I;

      Is_out.insert( Is_out.end(), Is_1.begin(), Is_1.end());
      Is_out.insert( Is_out.end(), Is_2.begin(), Is_2.end());
    }
  }

  //Infos
  cloud_out->name = "Merged cloud";
  col_out->file_format = ".pts";
  /*col_out->nb_point = cloud_out->xyz.size();

  //---------------------------
  if(col_out->nb_point > 0){
    col_out->list_obj.push_back(cloud_out);
    loaderManager->load_cloud_creation(col_out);
  }
  else{
    cout<<"No points available"<<endl;
  }*/
}
void Extraction::fct_merging_newCloud(Collection* cloud_1, Collection* cloud_2){
  //---------------------------

  //New cloud
  Collection* col_out = new Collection();
  Cloud* cloud_out = new Cloud();

  Cloud* subset_1 = (Cloud*)cloud_1->get_obj(0);
  Cloud* subset_2 = (Cloud*)cloud_2->get_obj(0);

  //Location
  vector<vec3>& XYZ_1 = subset_1->xyz;
  vector<vec3>& XYZ_2 = subset_2->xyz;
  vector<vec3>& XYZ_out = cloud_out->xyz;
  XYZ_out.insert( XYZ_out.end(), XYZ_1.begin(), XYZ_1.end());
  XYZ_out.insert( XYZ_out.end(), XYZ_2.begin(), XYZ_2.end());

  //Color
  if(subset_1->has_color && subset_2->has_color){
    vector<vec4>& RGB_1 = subset_1->rgb;
    vector<vec4>& RGB_2 = subset_2->rgb;
    vector<vec4>& RGB_out = cloud_out->rgb;

    RGB_out.insert( RGB_out.end(), RGB_1.begin(), RGB_1.end());
    RGB_out.insert( RGB_out.end(), RGB_2.begin(), RGB_2.end());

    cloud_out->has_color = true;
  }
  //Normal
  if(subset_1->Nxyz.size() != 0 && subset_2->Nxyz.size() != 0){
    vector<vec3>& N_1 = subset_1->Nxyz;
    vector<vec3>& N_2 = subset_2->Nxyz;
    vector<vec3>& N_out = cloud_out->Nxyz;

    N_out.insert( N_out.end(), N_1.begin(), N_1.end());
    N_out.insert( N_out.end(), N_2.begin(), N_2.end());
  }
  //Intensity
  if(subset_1->I.size() != 0 && subset_2->I.size() != 0){
    vector<float>& Is_1 = subset_1->I;
    vector<float>& Is_2 = subset_2->I;
    vector<float>& Is_out = cloud_out->I;

    Is_out.insert( Is_out.end(), Is_1.begin(), Is_1.end());
    Is_out.insert( Is_out.end(), Is_2.begin(), Is_2.end());
  }

  //Infos
  cloud_out->name = "Merging_" + subset_1->name + "_" + subset_2->name;
  col_out->file_format = ".pts";
  /*col_out->nb_point = cloud_out->xyz.size();

  //Create slice if any points
  if(col_out->nb_point != 0){
    col_out->list_obj.push_back(cloud_out);
    loaderManager->load_cloud_creation(col_out);
  }
  else{
    cout<<"No points available"<<endl;
  }*/

  //---------------------------
}
void Extraction::fct_merging_addCloud(Collection* cloud_1, Collection* cloud_2){
  //---------------------------

  Cloud* subset_1 = (Cloud*)cloud_1->get_obj(0);
  Cloud* subset_1_init = (Cloud*)cloud_1->get_obj_init_byID(0);
  Cloud* subset_2 = (Cloud*)cloud_2->get_obj(0);

  //Location
  vector<vec3>& XYZ_1 = subset_1->xyz;
  vector<vec3>& XYZ_2 = subset_2->xyz;
  XYZ_1.insert( XYZ_1.end(), XYZ_2.begin(), XYZ_2.end());
  subset_1_init->xyz = XYZ_1;
  //cloud_1->nb_point = XYZ_1.size();

  //Color
  if(subset_1->has_color && subset_2->has_color){
    vector<vec4>& RGB_1 = subset_1->rgb;
    vector<vec4>& RGB_2 = subset_2->rgb;
    RGB_1.insert( RGB_1.end(), RGB_2.begin(), RGB_2.end());
    subset_1_init->rgb = RGB_1;
  }
  //Normal
  if(subset_1->Nxyz.size() != 0 && subset_2->Nxyz.size() != 0){
    vector<vec3>& N_1 = subset_1->Nxyz;
    vector<vec3>& N_2 = subset_2->Nxyz;
    N_1.insert( N_1.end(), N_2.begin(), N_2.end());
    subset_1_init->Nxyz = N_1;
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
void Extraction::fct_highlighting(Cloud* cloud, Cloud* list_obj_init){
  vec3 max = cloud->max;
  vec3 min = cloud->min;
  vector<vec3>& pos = cloud->xyz;
  vector<vec4>& color = cloud->rgb;
  vector<vec4>& RGB = list_obj_init->rgb;
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
    cloud->rgb = RGB;
  }

  //---------------------------
  sceneManager->update_buffer_color(cloud);
}
void Extraction::set_AABB_min(vec3 min_in){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
  //---------------------------

  vec3 max_old = cloud->max;
  vec3 min_old = cloud->min;

  //Get Z extremums
  sceneManager->update_MinMax_col(collection);
  vec3 min = cloud->min;
  vec3 max = cloud->max;
  vec3 diff = max - min;
  vec3 min_out;

  for(int i=0; i<3; i++){
    if(min_in[i] > 100) min_in[i] = 100;
    if(min_in[i] <= 0) diff[i] = 0;
    else diff[i] = diff[i] * min_in[i]/100;

    min_out[i] = min[i] + diff[i];
    if(min_out[i] > max_old[i]) min_out[i] = max_old[i];
  }

  cloud->max = max_old;
  cloud->min = min_old;
  cloud->min = min_out;

  //---------------------------
  this->fct_highlighting(cloud, list_obj_init);
  objectManager->update_glyph_collection(collection);
}
void Extraction::set_AABB_max(vec3 max_in){
  Collection* collection = sceneManager->get_selected_collection();
  Cloud* cloud = (Cloud*)collection->selected_obj;
  Cloud* list_obj_init = (Cloud*)collection->get_obj_selected_init();
  //---------------------------

  vec3 max_old = cloud->max;
  vec3 min_old = cloud->min;

  //Get Z extremums
  sceneManager->update_MinMax_col(collection);
  vec3 min = cloud->min;
  vec3 max = cloud->max;
  vec3 diff = max - min;
  vec3 max_out;

  for(int i=0; i<3; i++){
    if(max_in[i] > 100) max_in[i] = 100;
    if(max_in[i] <= 0) diff[i] = 0;
    else diff[i] = diff[i] * max_in[i]/100;

    max_out[i] = min[i] + diff[i];
    if(max_out[i] < min_old[i]) max_out[i] = min_old[i];
  }

  cloud->max = max_old;
  cloud->min = min_old;
  cloud->max = max_out;

  //---------------------------
  this->fct_highlighting(cloud, list_obj_init);
}
