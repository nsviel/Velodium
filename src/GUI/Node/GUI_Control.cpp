#include "GUI_Control.h"

#include "../../Engine/Glyphs.h"
#include "../../Engine/Scene.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Data/Database.h"
#include "../../Engine/OpenGL/Camera.h"
#include "../../Engine/OpenGL/struct_viewport.h"
#include "../../Engine/Configuration/Dimension.h"

#include "../../Operation/Transformation/Attribut.h"
#include "../../Operation/Operation.h"
#include "../../Operation/Functions/Selection.h"
#include "../../Operation/Functions/Extraction.h"
#include "../../Operation/Functions/Heatmap.h"

#include "../../../extern/imgui/imgui.h"

extern struct Database database;


//Constructor / Destructor
GUI_control::GUI_control(Engine* engine){
  this->engineManager = engine;
  //---------------------------

  this->cameraManager = engineManager->get_CameraManager();
  this->dimManager = engineManager->get_dimManager();
  this->heatmapManager = engineManager->get_heatmapManager();

  this->selectionManager = new Selection(dimManager, cameraManager);
  this->sceneManager = new Scene();
  this->glyphManager = new Glyphs();
  this->extractionManager = new Extraction();
  this->attribManager = new Attribut();
  this->opeManager = new Operation();

  this->move_trans_speed = configuration.TRANSFORM_Trans;
  this->rotatDegree = configuration.TRANSFORM_Rot;
  this->wheel_mode = 0;

  //---------------------------
}
GUI_control::~GUI_control(){}

//Main function
void GUI_control::make_control(){
  //---------------------------

  this->control_mouse();
  this->control_frameSelection();
  this->control_keyboard_oneAction();
  this->control_keyboard_translation();
  this->control_keyboard_ctrlAction();
  this->control_keyboard_camMove();

  //---------------------------
}

//Subfunctions
void GUI_control::control_mouse(){
  Cloud* cloud = database.cloud_selected;
  ImGuiIO io = ImGui::GetIO();
  GLFWwindow* window = glfwGetCurrentContext();
  Viewport_obj* view = cameraManager->get_current_viewport();
  //----------------------------

  this->control_mouse_wheel();

  //Right click - Camera movement
  static vec2 cursorPos;
  if(ImGui::IsMouseClicked(1) && !io.WantCaptureMouse){

    //Save cursor position
    cursorPos = dimManager->get_cursorPos();

    //Hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //Set cursor to screen middle
    vec2 glMiddle = dimManager->get_glMiddle();
    glfwSetCursorPos(window, glMiddle.x, glMiddle.y);

    //Enable camera movement
    view->cameraMovON = true;
  }
  if(ImGui::IsMouseReleased(1) && cameraManager->is_cameraMovON()){
    //Restaure cursor position
    dimManager->set_cursorPos(cursorPos);

    //Disable camera movement
    view->cameraMovON = false;
  }
  if(io.MouseDown[1] && !io.WantCaptureMouse){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }

  //Left click - Point selection
  if(ImGui::IsMouseClicked(0) && !io.WantCaptureMouse){
    //vec3 point = selectionManager->mouse_clickedPoint();
    //selectionManager->selectionPoint(point);
  }

  //---------------------------
}
void GUI_control::control_mouse_wheel(){
  Cloud* cloud = database.cloud_selected;
  static int PCrotMode = 2;
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Wheel + right clicked - Camera zoom
  if(io.MouseWheel && io.MouseDownDuration[1] >= 0.0f && !io.WantCaptureMouse){
    cameraManager->compute_positionalZoom(io.MouseWheel);
  }

  //Wheel click - Change cloud rotation axis
  if(ImGui::IsMouseClicked(2) && !io.WantCaptureMouse){
    PCrotMode++;
    if(PCrotMode >= 3) PCrotMode = 0;
  }

  //----------------------------
}
void GUI_control::control_frameSelection(){
  Cloud* cloud = database.cloud_selected;
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Frame selection
  static vec2 pt1, pt2;
  if(ImGui::IsKeyPressed(341) == false){
    if(ImGui::IsMouseClicked(0) && io.MouseDownDuration[0] < 0.01f && !io.WantCaptureMouse){
      pt1 = dimManager->get_cursorPos_gl();
    }
    if(io.MouseDownDuration[0] >= 0.01f && !io.WantCaptureMouse){
      pt2 = dimManager->get_cursorPos_gl();
      selectionManager->mouse_drawFrame(pt1, pt2);
    }
    if(ImGui::IsMouseReleased(0) && !io.WantCaptureMouse){
      selectionManager->mouse_frameSelection(pt1, pt2);
    }
  }

  //----------------------------
}

void GUI_control::control_keyboard_oneAction(){
  Cloud* cloud = database.cloud_selected;
  ImGuiIO io = ImGui::GetIO();
  GLFWwindow* window = glfwGetCurrentContext();
  //----------------------------

  for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
    //Esc key
    if(ImGui::IsKeyPressed(256)){
      GLFWwindow* window = glfwGetCurrentContext();
      glfwSetWindowShouldClose(window, true);
    }

    //Tab key
    if (ImGui::IsKeyPressed(258)){
      bool* highlightON = extractionManager->get_highlightON();
      *highlightON = false;

      //Select the next cloud in the list
      sceneManager->selection_setNext();
      break;
    }

    //Suppr key - Delete selected
    if (ImGui::IsKeyPressed(261)){
      bool atLeastOne = selectionManager->mark_supressSelectedPoints_all();

      if(atLeastOne == false){
        Cloud* cloud = database.cloud_selected;
        sceneManager->removeCloud(cloud);
      }
      break;
    }

    //Fin key - Delete all
    if (ImGui::IsKeyPressed(269)){
      sceneManager->removeCloud_all();
      break;
    }

    //R key - Reset
    if (ImGui::IsKeyPressed(82) && !io.WantCaptureMouse){
      opeManager->reset();
      break;
    }

    //H key - Heatmap
    if (ImGui::IsKeyPressed(72) && !io.WantCaptureMouse){
      heatmapManager->set_Heatmap(cloud);
      break;
    }

    //5 key - Centering
    if (ImGui::IsKeyPressed(325) && !io.WantCaptureMouse){
      if(sceneManager->is_atLeastOnecloud()){
        transformManager.make_centering(cloud);
        sceneManager->update_cloud_location(cloud);
      }
      break;
    }

    //Enter - Validation
    if(ImGui::IsKeyPressed(257) && !io.WantCaptureMouse){
      selectionManager->validate();
      break;
    }

    //N key - Save all & remove all & load new
    if(ImGui::IsKeyPressed(78) && !io.WantCaptureMouse){
      opeManager->allSaving();
      sceneManager->removeCloud_all();
    }
  }

  //----------------------------
}
void GUI_control::control_keyboard_translation(){
  Cloud* cloud = database.cloud_selected;
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
    if(!io.MouseDown[1] && !io.WantCaptureMouse && cloud != nullptr){
      Subset* subset = &cloud->subset[cloud->subset_selected];

      //Shift speed up
      if(io.KeysDown[340]){
        transCoef = move_trans_speed * 5;
      }

      // Z key
      if(io.KeysDown[87]){
        vec3 translation = vec3(transCoef,0,0);

        transformManager.make_translation(subset, translation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // S key
      if(io.KeysDown[83]){
        vec3 translation = vec3(-transCoef,0,0);

        transformManager.make_translation(subset, translation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // D key
      if(io.KeysDown[68]){
        vec3 translation = vec3(0,transCoef,0);

        transformManager.make_translation(subset, translation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // Q key
      if(io.KeysDown[65]){
        vec3 translation = vec3(0,-transCoef,0);

        transformManager.make_translation(subset, translation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // A key
      if(io.KeysDown[81]){
        vec3 translation = vec3(0,0,transCoef);

        transformManager.make_translation(subset, translation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // E key
      if(io.KeysDown[69]){
        vec3 translation = vec3(0,0,-transCoef);

        transformManager.make_translation(subset, translation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // 7 key
      if(io.KeysDown[327]){
        float r = rotatDegree*M_PI/180;
        vec3 rotation = vec3(0,0,r);

        transformManager.make_rotation(subset, vec3(0,0,0), rotation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }
      // 9 key
      if(io.KeysDown[329]){
        float r = rotatDegree*M_PI/180;
        vec3 rotation = vec3(0,0,-r);

        transformManager.make_rotation(subset, vec3(0,0,0), rotation);
        sceneManager->update_subset_location(subset);
        sceneManager->update_subset_glyphs(subset);

        break;
      }

      transCoef = move_trans_speed;
    }
  }

  //----------------------------
}
void GUI_control::control_keyboard_ctrlAction(){
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //if ctrl
  if(ImGui::IsKeyPressed(341)){
    //ctrl+s - Save as
    if(ImGui::IsKeyPressed(83)){
      opeManager->saving();
    }
    //ctrl+w - Open
    if(ImGui::IsKeyPressed(90)){
      opeManager->loading();
    }
  }

  //----------------------------
}
void GUI_control::control_keyboard_camMove(){
  ImGuiIO io = ImGui::GetIO();
  Viewport_obj* view = cameraManager->get_current_viewport();
  //----------------------------

  if(view->cameraMovON){
    float delta = 0.00016;
    float camSpeed = view->cam_speed * delta;
    float fastSpeed = view->cam_speed * delta * 4;

    for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
      if(io.MouseDown[1] && !io.WantCaptureMouse){

        //Shift speed up
        if(io.KeysDown[340]){
          camSpeed = fastSpeed;
        }

        //Z key or Up key
        if(io.KeysDown[87] || io.KeysDown[265]){
          view->cam_P += view->cam_F * camSpeed;
        }

        //S key or Down key
        if(io.KeysDown[83] || io.KeysDown[264]){
          view->cam_P -= view->cam_F * camSpeed;
        }

        //Q key or Right key
        if(io.KeysDown[65] || io.KeysDown[263]){
          view->cam_P -= view->cam_R * camSpeed;
        }

        //D key or Left key
        if(io.KeysDown[68] || io.KeysDown[262]){
          view->cam_P += view->cam_R * camSpeed;
        }
      }
    }
  }

  //---------------------------
}