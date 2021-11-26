#include "Extraction.h"

#include "../Transformation/Attribut.h"

#include "../../Load/Loader.h"
#include "../../Engine/Scene.h"
#include "../../Engine/Data/Database.h"

extern struct Database database;



//Constructor / destructor
Extraction::Extraction(){
  //---------------------------

  this->sceneManager = new Scene();
  this->loaderManager = new Loader();
  this->attribManager = new Attribut();

  this->list_part = new list<subpart*>;
  this->highlightON = false;
  this->sliceON = false;
  this->ID_cloud = 0;
  this->ID_part = 0;

  //---------------------------
}
Extraction::~Extraction(){}

//Extraction
void Extraction::fct_extractCloud(Cloud* cloud){
  Subset* subset = &cloud->subset[cloud->subset_selected];
  Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
  //---------------------------

  Cloud* cloud_out = new Cloud();
  vector<vec3>& XYZ = cloud->subset[0].xyz;
  const vector<vec4>& RGB = subset_init->RGB;
  vector<vec3>& N = cloud->subset[0].N;
  vector<float>& Is = cloud->subset[0].I;
  vec3& max = cloud->subset[0].max;
  vec3& min = cloud->subset[0].min;
  cloud_out->format = ".pts";
  cloud_out->subset[0].name = cloud->subset[0].name + "_" + "part" + to_string(ID_cloud);
  ID_cloud++;
  //---------------------------

  //Take values between sliceMin and sliceMax
  for (int i=0; i<XYZ.size(); i++)
    if(XYZ[i].x >= min.x && XYZ[i].x <= max.x &&
      XYZ[i].y >= min.y && XYZ[i].y <= max.y &&
      XYZ[i].z >= min.z && XYZ[i].z <= max.z){
      //Location
      if(sliceON){
        cloud_out->subset[0].xyz.push_back(vec3(XYZ[i].x, XYZ[i].y,  min.z));
      }
      else{
        cloud_out->subset[0].xyz.push_back(vec3(XYZ[i].x, XYZ[i].y,  XYZ[i].z));
      }
      //Color
      if(cloud->subset[0].has_color){
        cloud_out->subset[0].RGB.push_back(vec4(RGB[i].x, RGB[i].y, RGB[i].z, RGB[i].w));
        cloud_out->subset[0].has_color = true;
      }
      //Normal
      if(cloud->subset[0].N.size() != 0){
        cloud_out->subset[0].N.push_back(vec3(N[i].x, N[i].y, N[i].z));
      }
      //Intensity
      if(cloud->subset[0].I.size() != 0){
        cloud_out->subset[0].I.push_back(Is[i]);
      }
    }

  //---------------------------
  if(cloud_out->subset[0].xyz.size() != 0){
    //Create slice if any points
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points detected"<<endl;
  }
}
void Extraction::fct_extractSelected(Cloud* cloud){
  Subset* subset = &cloud->subset[cloud->subset_selected];
  Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
  //---------------------------

  Cloud* cloud_out = new Cloud();
  vector<vec3>& XYZ = cloud->subset[0].xyz;
  const vector<vec4>& RGB = subset_init->RGB;
  vector<vec3>& N = cloud->subset[0].N;
  vector<float>& Is = cloud->subset[0].I;

  cloud_out->format = ".pts";
  cloud_out->subset[0].name = cloud->subset[0].name + "_" + "part" + to_string(ID_cloud);
  ID_cloud++;
  vector<int>& idx = cloud->subset[0].selected;
  //---------------------------

  for(int i=0; i<idx.size(); i++){
    cloud_out->subset[0].xyz.push_back(vec3(XYZ[idx[i]].x, XYZ[idx[i]].y,  XYZ[idx[i]].z));

    //Color
    if(cloud->subset[0].has_color){
      cloud_out->subset[0].RGB.push_back(vec4(RGB[idx[i]].x, RGB[idx[i]].y, RGB[idx[i]].z, RGB[idx[i]].w));
      cloud_out->subset[0].has_color = true;
    }

    //Normal
    if(cloud->subset[0].N.size() != 0){
      cloud_out->subset[0].N.push_back(vec3(N[idx[i]].x, N[idx[i]].y, N[idx[i]].z));
    }

    //Intensity
    if(cloud->subset[0].I.size() != 0){
      cloud_out->subset[0].I.push_back(Is[idx[i]]);
    }
  }

  //---------------------------
  idx.clear();
  if(cloud_out->subset[0].xyz.size() != 0){
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
  list<Cloud*>* list_Cloud = database.list_cloud;
  Cloud* cloud_selected = database.cloud_selected;
  vec3& max = cloud_selected->max;
  vec3& min = cloud_selected->min;
  //---------------------------

  for(int i=0;i<list_Cloud->size();i++){
    //Select ieme Point Cloud
    Cloud* cloud = *next(list_Cloud->begin(),i);
    Subset* subset = &cloud->subset[cloud->subset_selected];

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
void Extraction::fct_merging_list(vector<Cloud*> list_part){
  Cloud* part_1;
  Cloud* part_2;
  Cloud* cloud_out = new Cloud();
  //---------------------------

  for(int i=0; i<list_part.size()-1; i++){
    part_1 = list_part[i];
    part_2 = list_part[i+1];

    //Location
    vector<vec3>& XYZ_1 = part_1->subset[0].xyz;
    vector<vec3>& XYZ_2 = part_2->subset[0].xyz;
    vector<vec3>& XYZ_out = cloud_out->subset[0].xyz;

    XYZ_out.insert( XYZ_out.end(), XYZ_1.begin(), XYZ_1.end());
    XYZ_out.insert( XYZ_out.end(), XYZ_2.begin(), XYZ_2.end());

    //Color
    if(part_1->subset[0].has_color && part_2->subset[0].has_color){
      vector<vec4>& RGB_1 = part_1->subset[0].RGB;
      vector<vec4>& RGB_2 = part_2->subset[0].RGB;
      vector<vec4>& RGB_out = cloud_out->subset[0].RGB;

      RGB_out.insert( RGB_out.end(), RGB_1.begin(), RGB_1.end());
      RGB_out.insert( RGB_out.end(), RGB_2.begin(), RGB_2.end());

      cloud_out->subset[0].has_color = true;
    }
    //Normal
    if(part_1->subset[0].N.size() != 0 && part_2->subset[0].N.size() != 0){
      vector<vec3>& N_1 = part_1->subset[0].N;
      vector<vec3>& N_2 = part_2->subset[0].N;
      vector<vec3>& N_out = cloud_out->subset[0].N;

      N_out.insert( N_out.end(), N_1.begin(), N_1.end());
      N_out.insert( N_out.end(), N_2.begin(), N_2.end());
    }
    //Intensity
    if(part_1->subset[0].I.size() != 0 && part_2->subset[0].I.size() != 0){
      vector<float>& Is_1 = part_1->subset[0].I;
      vector<float>& Is_2 = part_2->subset[0].I;
      vector<float>& Is_out = cloud_out->subset[0].I;

      Is_out.insert( Is_out.end(), Is_1.begin(), Is_1.end());
      Is_out.insert( Is_out.end(), Is_2.begin(), Is_2.end());
    }
  }

  //Infos
  cloud_out->subset[0].name = part_1->subset[0].name;
  cloud_out->format = ".pts";
  cloud_out->nb_point = cloud_out->subset[0].xyz.size();

  //---------------------------
  if(cloud_out->nb_point > 0){
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points available"<<endl;
  }
}
void Extraction::fct_merging_newCloud(Cloud* cloud_1, Cloud* cloud_2){
  Cloud* cloud_out = new Cloud();
  //---------------------------

  //Location
  vector<vec3>& XYZ_1 = cloud_1->subset[0].xyz;
  vector<vec3>& XYZ_2 = cloud_2->subset[0].xyz;
  vector<vec3>& XYZ_out = cloud_out->subset[0].xyz;
  XYZ_out.insert( XYZ_out.end(), XYZ_1.begin(), XYZ_1.end());
  XYZ_out.insert( XYZ_out.end(), XYZ_2.begin(), XYZ_2.end());

  //Color
  if(cloud_1->subset[0].has_color && cloud_2->subset[0].has_color){
    vector<vec4>& RGB_1 = cloud_1->subset[0].RGB;
    vector<vec4>& RGB_2 = cloud_2->subset[0].RGB;
    vector<vec4>& RGB_out = cloud_out->subset[0].RGB;

    RGB_out.insert( RGB_out.end(), RGB_1.begin(), RGB_1.end());
    RGB_out.insert( RGB_out.end(), RGB_2.begin(), RGB_2.end());

    cloud_out->subset[0].has_color = true;
  }
  //Normal
  if(cloud_1->subset[0].N.size() != 0 && cloud_2->subset[0].N.size() != 0){
    vector<vec3>& N_1 = cloud_1->subset[0].N;
    vector<vec3>& N_2 = cloud_2->subset[0].N;
    vector<vec3>& N_out = cloud_out->subset[0].N;

    N_out.insert( N_out.end(), N_1.begin(), N_1.end());
    N_out.insert( N_out.end(), N_2.begin(), N_2.end());
  }
  //Intensity
  if(cloud_1->subset[0].I.size() != 0 && cloud_2->subset[0].I.size() != 0){
    vector<float>& Is_1 = cloud_1->subset[0].I;
    vector<float>& Is_2 = cloud_2->subset[0].I;
    vector<float>& Is_out = cloud_out->subset[0].I;

    Is_out.insert( Is_out.end(), Is_1.begin(), Is_1.end());
    Is_out.insert( Is_out.end(), Is_2.begin(), Is_2.end());
  }

  //Infos
  cloud_out->subset[0].name = "Merging_" + cloud_1->subset[0].name + "_" + cloud_2->subset[0].name;
  cloud_out->format = ".pts";
  cloud_out->nb_point = cloud_out->subset[0].xyz.size();

  //Create slice if any points
  if(cloud_out->nb_point != 0){
    loaderManager->load_cloud_creation(cloud_out);
  }
  else{
    cout<<"No points available"<<endl;
  }

  //---------------------------
}
void Extraction::fct_merging_addCloud(Cloud* cloud_1, Cloud* cloud_2){
  //---------------------------

  //Location
  vector<vec3>& XYZ_1 = cloud_1->subset[0].xyz;
  vector<vec3>& XYZ_2 = cloud_2->subset[0].xyz;
  XYZ_1.insert( XYZ_1.end(), XYZ_2.begin(), XYZ_2.end());
  cloud_1->subset_init[0].xyz = XYZ_1;
  cloud_1->nb_point = XYZ_1.size();

  //Color
  if(cloud_1->subset[0].has_color && cloud_2->subset[0].has_color){
    vector<vec4>& RGB_1 = cloud_1->subset[0].RGB;
    vector<vec4>& RGB_2 = cloud_2->subset[0].RGB;
    RGB_1.insert( RGB_1.end(), RGB_2.begin(), RGB_2.end());
    cloud_1->subset_init[0].RGB = RGB_1;
  }
  //Normal
  if(cloud_1->subset[0].N.size() != 0 && cloud_2->subset[0].N.size() != 0){
    vector<vec3>& N_1 = cloud_1->subset[0].N;
    vector<vec3>& N_2 = cloud_2->subset[0].N;
    N_1.insert( N_1.end(), N_2.begin(), N_2.end());
    cloud_1->subset_init[0].N = N_1;
  }
  //Intensity
  if(cloud_1->subset[0].I.size() != 0 && cloud_2->subset[0].I.size() != 0){
    vector<float>& Is_1 = cloud_1->subset[0].I;
    vector<float>& Is_2 = cloud_2->subset[0].I;
    Is_1.insert( Is_1.end(), Is_2.begin(), Is_2.end());
    cloud_1->subset_init[0].I = Is_1;
  }

  //---------------------------
}
void Extraction::set_AABB_min(vec3 min_in){
  Cloud* cloud = database.cloud_selected;
  Subset* subset = &cloud->subset[cloud->subset_selected];
  Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
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
}
void Extraction::set_AABB_max(vec3 max_in){
  Cloud* cloud = database.cloud_selected;
  Subset* subset = &cloud->subset[cloud->subset_selected];
  Subset* subset_init = &cloud->subset_init[cloud->subset_selected];
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

//Selection subparts
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
void Extraction::supress_selectedpart(subpart* part){
  //---------------------------

  if(list_part->size() != 0){
    int ID = part->ID;

    list<subpart*>::iterator it = next(list_part->begin(), ID);
    list_part->erase(it);
  }

  //---------------------------
}
