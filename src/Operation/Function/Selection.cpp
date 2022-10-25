#include "Selection.h"

#include "CoordTransform.h"

#include "../Node_operation.h"
#include "../Transformation/Transforms.h"
#include "../Transformation/Attribut.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Object.h"
#include "../../Engine/Scene/Object/Scene/Mark.h"
#include "../../Engine/OpenGL/Camera/Camera.h"
#include "../../Engine/OpenGL/Camera/Renderer.h"
#include "../../Engine/OpenGL/Dimension.h"
#include "../../Specific/fct_maths.h"

#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


//Constructor / Destructor
Selection::Selection(Node_operation* node_ope){
  //---------------------------

  Node_engine* node_engine = node_ope->get_node_engine();

  this->dimManager = node_engine->get_dimManager();
  this->cameraManager = node_engine->get_cameraManager();
  this->renderManager = node_engine->get_renderManager();
  this->coordManager = node_ope->get_coordManager();
  this->transformManager = new Transforms();
  this->attribManager = node_ope->get_attribManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->objectManager = node_engine->get_objectManager();

  this->gui_X = 0;
  this->gui_Y = 0;
  this->nbMark = 0;
  this->ID_plane = -1;
  this->selectSensibility = 0.004f;
  this->markMode = "cube";
  this->planeMark = new Cloud();

  //---------------------------
}
Selection::~Selection(){}

//Drawing functions
void Selection::update(){
  //---------------------------

  this->mark_pointLocation();
  //this->mark_planeLocation();

  //---------------------------
}
void Selection::validate(){
  if(!sceneManager->get_is_list_empty()){
    Cloud* cloud = sceneManager->get_selected_cloud();
    Subset* subset = cloud->subset_selected;
    Subset* subset_init = sceneManager->get_subset_selected_init();
    list<int>& idx = subset->highlighted;
    //---------------------------

    //Plane normal computation
    if(idx.size() == 0){
      this->mark_planeABpoints(cloud);
      float angle = transformManager->make_orientAxis_X(cloud);
      attribManager->compute_normals(subset);
      vec3 rotation = vec3(0, 0, -angle);
      transformManager->make_rotation(cloud, subset->COM, rotation);
      subset_init->N = subset->N;
      sceneManager->update_cloud_location(cloud);
    }

    //---------------------------
  }
}

//Point selection
void Selection::selectionPoint(vec3 point){
  //---------------------------

  //If selected point already exist, suppress the mark
  bool ptExist = this->mark_pointSupression(point);

  //else create a new mark
  if(!ptExist) this->mark_pointCreation(point);

  //---------------------------
}
void Selection::mark_pointCreation(vec3 point){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  float err = selectSensibility;
  //---------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;
    Subset* subset_init = sceneManager->get_subset_selected_init();

    vector<vec3>& XYZ = subset->xyz;

    for(int j=0; j<XYZ.size(); j++){
      if(point.x <= XYZ[j].x + err && point.x >= XYZ[j].x - err &&
         point.y <= XYZ[j].y + err && point.y >= XYZ[j].y - err &&
         point.z <= XYZ[j].z + err && point.z >= XYZ[j].z - err){
        vector<float>& Is = subset->I;
        const vector<float>& Is_ini = subset_init->I;
        vector<float>& It = subset->It;

        //Give information about point
        if(It.size() == 0 && subset->N.size() != 0){
          attribManager->compute_cosIt(subset);
        }
        if(Is.size() != 0 && It.size() != 0){
          float dist = fct_distance(XYZ[j], vec3(0,0,0));
          cout<<XYZ[j].x<<" "<<XYZ[j].y<<" "<<XYZ[j].z;
          cout<<" ("<<dist<<" m)";
          cout<<" -> I_ini= "<<Is_ini[j];
          cout<<" -> I_obj= "<<Is[j];
          cout<<" -> It= "<<It[j]<<endl;
        }else if(Is.size() != 0){
          float dist = fct_distance(point, vec3(0,0,0));
          cout<<XYZ[j].x<<" "<<XYZ[j].y<<" "<<XYZ[j].z;
          cout<<" ("<<dist<<" m)";
          cout<<" -> I= "<<Is[j]<<endl;
        }else{
          float dist = fct_distance(point, vec3(0,0,0));
          cout<<"-> Pt : "<<XYZ[j].x<<" "<<XYZ[j].y<<" "<<XYZ[j].z;
          cout<<" ("<<dist<<" m)"<<endl;
        }

        //Clear point list if no marks
        list<int>& idx = subset->highlighted;
        if(list_glyph.size() == 0){
          idx.clear();
        }
        idx.push_back(j);

        if(markMode == "cube"){
          //int ID = objectManager->loadGlyph("../media/engine/Marks/cube.pts", point, "point", false);
          //objectManager->update_glyph_color(ID, vec3(1.0f, 1.0f, 1.0f));
          //list_glyph.push_back(ID);
        }
        if(markMode == "sphere"){
          //int ID = objectManager->loadGlyph("../media/engine/Marks/sphere.pts", point, "point", false);
          //list_glyph.push_back(ID);
        }

        break;
      }
    }
  }

  //---------------------------
}
bool Selection::mark_pointSupression(vec3 point){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  float err = selectSensibility;
  //---------------------------

  //For each cloud
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    vector<vec3>& XYZ = subset->xyz;
    list<int>& idx = subset->highlighted;

    //If point
    for(int j=0; j<idx.size(); j++){
      int idx_j = *next(idx.begin(), j);
      if(point.x <= XYZ[idx_j].x + err && point.x >= XYZ[idx_j].x - err &&
         point.y <= XYZ[idx_j].y + err && point.y >= XYZ[idx_j].y - err &&
         point.z <= XYZ[idx_j].z + err && point.z >= XYZ[idx_j].z - err){

        list<int>::iterator it_idx = next(idx.begin(), j);
        idx.erase(it_idx);

        int ID = *next(list_glyph.begin(), 0);
        //objectManager->remove_glyph_scene(ID);
        list<int>::iterator it = next(list_glyph.begin(), 0);
        list_glyph.erase(it);

        return true;
      }
    }
  }

  //---------------------------
  return false;
}
void Selection::mark_pointColor(Cloud* ptMark, int num){
  float R, G, B;
  //---------------------------

  switch(num){
    case 0:{
      R = 0.0f;
      G = 0.0f;
      B = 1.0f;
      break;
    }
    case 1:{
      R = 0.0f;
      G = 1.0f;
      B = 0.0f;
      break;
    }
    case 2:{
      R = 1.0f;
      G = 1.0f;
      B = 1.0f;
      break;
    }
    default:{
      R = 1.0f;
      G = 0.0f;
      B = 0.0f;
      break;
    }
  }

  Subset* subset = *next(ptMark->subset.begin(), 0);
  vector<vec4>& RGB = subset->RGB;
  for(int i=0; i<RGB.size(); i++){
    RGB[i] = vec4(R, G, B, 1.0f);
  }

  //---------------------------
}
void Selection::mark_supressAll(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    list<int>& idx = subset->highlighted;
    idx.clear();
  }

  //---------------------------
}
void Selection::mark_supressSelectedPoints_all(){
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  for(int i=0;i<list_cloud->size();i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    vector<int>& idx = subset->selected;

    if(idx.size() != 0){
      this->mark_supressSelectedPoints(cloud);
    }
  }

  //---------------------------
}
void Selection::mark_supressSelectedPoints(Cloud* cloud){
  for(int i=0; i<cloud->nb_subset; i++){
    Subset* subset = sceneManager->get_subset(cloud, i);
    vector<int>& idx = subset->selected;
    //---------------------------

    if(idx.size() != 0){
      attribManager->make_supressPoints(subset, idx);
      idx.clear();
    }

    //---------------------------
  }
}
void Selection::mark_pointLocation(){
  /*list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //Reposionning of ptMark if cloud move
  int cpt = 0;
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    list<int>& idx = subset->highlighted;
    vector<vec3>& XYZ = subset->xyz;

    //Points marks
    if(idx.size() <= list_glyph.size()){
      for(int j=0; j<idx.size(); j++){
        int idx_ = *next(idx.begin(),j);
        int ID = *next(list_glyph.begin(),cpt);

        Glyph* glyph = objectManager->get_glyph(ID);
        objectManager->update_glyph_MinMax(glyph);
        transformManager->make_positionning_glyph(glyph->location, glyph->COM, XYZ[idx_]);
        objectManager->update_glyph_location(glyph);

        cpt++;
      }
    }
  }

  //If too much ptMark, supress them
  while(cpt < list_glyph.size()){
    int ID = *next(list_glyph.begin(), 0);
    objectManager->remove_glyph_scene(ID);
    list<int>::iterator it = next(list_glyph.begin(), 0);
    list_glyph.erase(it);
  }*/

  //---------------------------
}

//Mouse interactivity
vec3 Selection::mouse_click_point(){
  this->update_glDims();
  //---------------------------

  //Get viewport mouse location
  vec2 curPos = dimManager->get_mouse_pose();
  vec3 mouse_pos = vec3(curPos.x, gl_Y - curPos.y + gui_Y, 0.0f);
  vec3 point = vec3(-1, -1, -1);

  //Get world location
  glReadPixels(mouse_pos.x, mouse_pos.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_pos.z);
  if((mouse_pos.z > 0) && (mouse_pos.z < 1)){
    glm::tvec4<unsigned int> viewport(gui_X, 0, gl_X, gl_Y);
    mat4 viewMat = cameraManager->get_viewMat();
    mat4 projMat = cameraManager->get_projMat();
    point = glm::unProject(mouse_pos, viewMat, projMat, viewport);
  }

  //---------------------------
  if(point.x != -1 && point.y != -1 && point.z != -1){
    cout<<"Any selected point.."<<endl;
  }
  return point;
}
void Selection::mouse_selection(int selectMouseMode){
  //---------------------------

  switch(selectMouseMode){
    case 0:{
      this->mouse_cloudPicking();
      break;
    }
    case 1:{
      //this->mouse_frameSelection();
      break;
    }
  }

  //---------------------------
}
void Selection::mouse_frameSelection(vec2 point1, vec2 point2){
  //---------------------------

  if(point1.x > point2.x){
    float pt = point1.x;
    point1.x = point2.x;
    point2.x = pt;
  }
  if(point1.y > point2.y){
    float pt = point1.y;
    point1.y = point2.y;
    point2.y = pt;
  }

  //Search and colorize selected points
  list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);

    for(int j=0; j<cloud->nb_subset; j++){
      Subset* subset = *next(cloud->subset.begin(), j);
      Subset* subset_buf = *next(cloud->subset_buffer.begin(), j);

      if(subset->visibility){
        vector<vec3>& XYZ = subset->xyz;
        vector<vec4>& RGB = subset->RGB;
        vector<int>& idx = subset->selected;
        vector<vec4>& RGB_buf = subset_buf->RGB;
        idx.clear();

        //Make selection
        for(int k=0; k<XYZ.size(); k++){
          vec2 projPT = coordManager->WorldToScreen(XYZ[k]);

          if(projPT.x >= point1.x && projPT.y >= point1.y && projPT.x <= point2.x && projPT.y <= point2.y){
            RGB[k] = vec4(1.0f,1.0f,1.0f,1.0f);
            idx.push_back(k);
          }else{
            RGB[k] = RGB_buf[k];
          }
        }

        sceneManager->update_subset_color(subset);
      }
    }
  }

  Mark* markObject = objectManager->get_object_mark();
  objectManager->reset_object(markObject->get_selection_frame());

  //--------------------------
}
void Selection::mouse_drawFrame(vec2 point1, vec2 point2){
  vec2 point3 = vec2(point1.x, point2.y);
  vec2 point4 = vec2(point2.x, point1.y);
  //---------------------------

  vec3 pt1 = coordManager->ScreenToWorld(point1);
  vec3 pt2 = coordManager->ScreenToWorld(point2);
  vec3 pt3 = coordManager->ScreenToWorld(point3);
  vec3 pt4 = coordManager->ScreenToWorld(point4);

  //New frame location
  vector<vec3> xyz;
  xyz.push_back(pt1);
  xyz.push_back(pt3);
  xyz.push_back(pt3);
  xyz.push_back(pt2);
  xyz.push_back(pt1);
  xyz.push_back(pt4);
  xyz.push_back(pt4);
  xyz.push_back(pt2);

  //Update frame glyph
  Mark* markObject = objectManager->get_object_mark();
  Glyph* frame = markObject->get_selection_frame();
  frame->location = xyz;
  objectManager->update_object(frame);

  //---------------------------
}
void Selection::mouse_cloudPicking(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //---------------------------

  vec3 point = coordManager->CursorToGround();
  transformManager->make_positionning_XY(cloud, point);

  //---------------------------
  sceneManager->update_cloud_location(cloud);
}
vec3 Selection::mouse_cameraPt(){
  vec2 cursorPos = dimManager->get_mouse_pose();
  //---------------------------

  //Get ray direction
  float x = (2.0f * cursorPos.x) / gl_X - 1.0f;
  float y = 1.0f - (2.0f * cursorPos.y) / gl_Y;
  float z = 1.0f;
  vec3 ray_nds = vec3(x, y, z);
  vec4 ray_clip = vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
  mat4 projMat = cameraManager->get_projMat();
  vec4 ray_eye = inverse(projMat) * ray_clip;
  ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);
  mat4 viewMat = cameraManager->get_viewMat();
  vec4 ray_wor = inverse(viewMat) * ray_eye;
  vec3 ray_world = vec3(ray_wor);
  vec3 ray_dir = normalize(ray_world);

  vec3 cam_pos = cameraManager->get_camPos();
  vec3 cam_forw = cameraManager->get_camForward();
  vec3 frustum = cam_pos + vec3(cam_forw.x*0.5,cam_forw.y*0.5,cam_forw.z*0.5);

  vec3 D = ray_dir;
  vec3 N = cam_forw;
  vec3 X = cam_pos + D * dot(frustum - cam_pos, N) / dot(D, N);

  //---------------------------
  return X;
}
void Selection::update_glDims(){
  vec2 glDim = dimManager->get_gl_dim();
  GLFWwindow* window = dimManager->get_window();
  //---------------------------

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  //this->gui_X = ImGui::GetWindowSize().x;
  //this->gui_Y = ImGui::GetWindowSize().y;
  this->gui_X = width;
  this->gui_Y = height;
  this->gl_X = glDim.x;
  this->gl_Y = glDim.y;

  //---------------------------
}

//Plane
void Selection::mark_planeCreation(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Subset* subset = cloud->subset_selected;
  //---------------------------

  list<int>& idx = subset->highlighted;
  vector<vec3> XYZ;
  vector<vec4> RGB;

  int i0 = *next(idx.begin(), 0);
  int i1 = *next(idx.begin(), 1);
  A = subset->xyz[i0];
  C = subset->xyz[i1];

  //infere more points
  B = vec3(C.x, C.y, A.z);
  D = vec3(A.x, A.y, C.z);

  //Plane coordinates
  XYZ.push_back(A);
  XYZ.push_back(B);
  XYZ.push_back(C);
  XYZ.push_back(C);
  XYZ.push_back(D);
  XYZ.push_back(A);

  //Plane color
  for(int i=0; i<XYZ.size(); i++){
    RGB.push_back(vec4(0.0f, 0.0f, 1.0f, 1.0f));
  }

  //---------------------------
  //ID_plane = objectManager->create_glyph(XYZ, RGB, "triangle", false);
}
void Selection::mark_planeABpoints(Cloud* cloud){
  Subset* subset = cloud->subset_selected;
  //---------------------------

  //Retrieve A & B points
  vector<vec3>& XYZ = subset->xyz;
  A = XYZ[0];
  B = XYZ[0];

  for(int i=0; i<XYZ.size(); i++){
    if(XYZ[i].x > A.x){
      A = XYZ[i];
    }
    if(XYZ[i].x < B.x){
      B = XYZ[i];
    }
  }

  //---------------------------
}
void Selection::mark_planeLocation(){
  /*list<Cloud*>* list_cloud = sceneManager->get_list_cloud();
  //---------------------------

  //For each Cloud insert plane
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    list<int>& idx = subset->highlighted;
    if(idx.size() == 2){
      this->mark_planeCreation();
    }else if(ID_plane != -1){
      objectManager->remove_glyph_scene(ID_plane);
    }
  }

  //Reposionning of plane if cloud move
  int cpt = 0;
  for(int i=0; i<list_cloud->size(); i++){
    Cloud* cloud = *next(list_cloud->begin(),i);
    Subset* subset = cloud->subset_selected;

    list<int>& idx = subset->highlighted;
    vector<vec3>& XYZ = subset->xyz;

    //Plane marks
    if(ID_plane != -1){
      Glyph* glyph = objectManager->get_glyph(ID_plane);
      vector<vec3>& XYZ_plane = glyph->location;
      if(idx.size() >= 2 && XYZ_plane.size() != 0){
        int i0 = *next(idx.begin(), 0);
        int i1 = *next(idx.begin(), 1);
        vec3 A = XYZ[i0];
        vec3 C = XYZ[i1];
        vec3 B = vec3(C.x, C.y, A.z);
        vec3 D = vec3(A.x, A.y, C.z);

        XYZ_plane[0] = A;
        XYZ_plane[1] = B;
        XYZ_plane[2] = C;
        XYZ_plane[3] = C;
        XYZ_plane[4] = D;
        XYZ_plane[5] = A;

        objectManager->update_glyph_location(glyph);
      }
    }
  }
  */
  //---------------------------
}
