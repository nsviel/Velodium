#include "GUI_Control.h"

#include "../Node_gui.h"
#include "../Modal/Modal_tab.h"
#include "../Dynamic/GUI_Player.h"

#include "../../Load/Node_load.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Camera/struct_viewport.h"
#include "../../Engine/Core/Dimension.h"
#include "../../Engine/Scene/Glyph/Glyphs.h"
#include "../../Engine/Scene/Scene.h"
#include "../../Engine/Scene/Configuration.h"

#include "../../Operation/Node_operation.h"
#include "../../Operation/Dynamic/Player.h"
#include "../../Operation/Transformation/Attribut.h"
#include "../../Operation/Transformation/Transformation.h"
#include "../../Operation/Transformation/Pose.h"
#include "../../Operation/Cloud/Selection.h"
#include "../../Operation/Cloud/Extraction.h"
#include "../../Operation/Cloud/Boxing.h"
#include "../../Operation/Color/Heatmap.h"

#include <glm/gtc/matrix_transform.hpp>
#include "imgui/imgui.h"

extern struct Modal_tab modal_tab;


//Constructor / Destructor
GUI_Control::GUI_Control(Node_gui* node){
  this->node_gui = node;
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();
  Node_engine* node_engine = node_gui->get_node_engine();
  Configuration* configManager = node_engine->get_configManager();
  Node_load* node_load = node_engine->get_node_load();

  this->cameraManager = node_engine->get_cameraManager();
  this->dimManager = node_engine->get_dimManager();
  this->heatmapManager = node_ope->get_heatmapManager();
  this->selectionManager = node_ope->get_selectionManager();
  this->sceneManager = node_engine->get_sceneManager();
  this->glyphManager = node_engine->get_glyphManager();
  this->attribManager = node_ope->get_attribManager();
  this->extractionManager = node_ope->get_extractionManager();
  this->pathManager = node_load->get_patherManager();
  this->playerManager = node_ope->get_playerManager();
  this->boxingManager = node_ope->get_boxingManager();
  this->transformManager = new Transformation();
  this->poseManager = new Pose();

  this->cloud_trans_speed = configManager->parse_json_f("parameter", "cloud_translation");
  this->cloud_rotat_degree = configManager->parse_json_f("parameter", "cloud_rotation");

  //---------------------------
}
GUI_Control::~GUI_Control(){}

//Main function
void GUI_Control::make_control(){
  //---------------------------

  this->control_mouse();
  this->control_frame_selection();
  this->control_keyboard_oneAction();
  this->control_keyboard_translation();
  this->control_keyboard_ctrlAction();
  this->control_keyboard_camMove();

  //---------------------------
}

//Mouse function
void GUI_Control::control_mouse(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  ImGuiIO io = ImGui::GetIO();
  GLFWwindow* window = glfwGetCurrentContext();
  Viewport_obj* view = cameraManager->get_current_viewport();
  //----------------------------

  this->control_mouse_wheel();

  //Right click - Camera movement
  static vec2 cursorPos;
  if(ImGui::IsMouseClicked(1) && !io.WantCaptureMouse){
    //Save cursor position
    cursorPos = dimManager->get_mouse_pose();

    //Hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    //Set cursor to screen middle
    vec2 glMiddle = dimManager->get_gl_middle();
    glfwSetCursorPos(window, glMiddle.x, glMiddle.y);

    //Enable camera movement
    view->cam_move = true;
  }
  //Right click release
  if(ImGui::IsMouseReleased(1) && cameraManager->is_cameraMovON()){
    //Restaure cursor position
    dimManager->set_mouse_pose(cursorPos);

    //Disable camera movement
    view->cam_move = false;
  }
  if(io.MouseDown[1] && !io.WantCaptureMouse){
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }

  //---------------------------
}
void GUI_Control::control_mouse_wheel(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  static int wheelMode = 0;
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Wheel + right clicked - Camera zoom
  if(io.MouseWheel && io.MouseDownDuration[1] >= 0.0f && !io.WantCaptureMouse){
    cameraManager->compute_zoom_position(io.MouseWheel);
  }

  //Wheel click - Change mouse wheel mode
  if(ImGui::IsMouseClicked(2) && !io.WantCaptureMouse){
    wheelMode++;
    if(wheelMode >= 3) wheelMode = 0;
  }

  //Wheel actions
  if(io.MouseWheel && io.MouseDownDuration[1] == -1 && !io.WantCaptureMouse){
    //Get wheel direction
    string direction;
    if(io.MouseWheel > 0){
      direction = "up";
    }else{
      direction = "down";
    }

    //Subset rotation
    if(sceneManager->get_is_list_empty() == false){
      if(cloud->nb_subset == 1 && cloud->onthefly == false){
        float radian = cloud_rotat_degree*M_PI/180;
        vec3 R;
        if(wheelMode == 0){
          R = vec3(0, 0, radian);
        }
        else if(wheelMode == 1){
          R = vec3(0, radian, 0);
        }
        else if(wheelMode == 2){
          R = vec3(radian, 0, 0);
        }

        poseManager->compute_COM(cloud);
        transformManager->make_rotation(cloud, R, direction);
        sceneManager->update_subset_location(cloud->subset_selected);
        sceneManager->update_cloud_glyph(cloud);
      }
      //Subset selection
      else if(cloud->nb_subset > 1 || cloud->onthefly){
        playerManager->compute_wheel_selection(direction);
      }
    }
  }

  //----------------------------
}
void GUI_Control::control_frame_selection(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //Frame selection
  static vec2 pt1, pt2;
  static bool frame_selection = false;
  if(ImGui::IsKeyPressed(341) == false){
    if(ImGui::IsMouseClicked(0) && io.MouseDownDuration[0] < 0.01f && !io.WantCaptureMouse){
      pt1 = dimManager->get_cursorPos_gl();
    }
    if(io.MouseDownDuration[0] >= 0.01f && !io.WantCaptureMouse){
      pt2 = dimManager->get_cursorPos_gl();
      selectionManager->mouse_drawFrame(pt1, pt2);
      frame_selection = true;
    }
    if(ImGui::IsMouseReleased(0) && frame_selection){
      selectionManager->mouse_frameSelection(pt1, pt2);
      frame_selection = false;
    }
  }

  //----------------------------
}

//Keyboard function
void GUI_Control::control_keyboard_oneAction(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  ImGuiIO io = ImGui::GetIO();
  GLFWwindow* window = glfwGetCurrentContext();
  //----------------------------

  for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
    //Esc key
    if(ImGui::IsKeyPressed(256)){
      node_gui->exit();
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
      this->key_suppr();
      break;
    }

    //Fin key - Delete all
    if (ImGui::IsKeyPressed(269)){
      sceneManager->remove_cloud_all();
      break;
    }

    //R key - Reset
    if (ImGui::IsKeyPressed(82) && !io.WantCaptureMouse){
      node_gui->reset();
      break;
    }

    //H key - Heatmap
    if (ImGui::IsKeyPressed(72) && !io.WantCaptureMouse){
      heatmapManager->make_cloud_heatmap(cloud);
      break;
    }

    //C key - Centering
    if (ImGui::IsKeyPressed(67) && !io.WantCaptureMouse){
      this->key_c();
      break;
    }

    //Enter - Validation
    if(ImGui::IsKeyPressed(257) && !io.WantCaptureMouse){
      selectionManager->validate();
      break;
    }

    //N key - Save all & remove all & load new
    if(ImGui::IsKeyPressed(78) && !io.WantCaptureMouse){
      //pathManager->saving_allCloud();
      //sceneManager->remove_cloud_all();
    }

    //o key - Open options
    if(ImGui::IsKeyPressed(79) && !io.WantCaptureMouse){
      modal_tab.show_loading = !modal_tab.show_loading;
      break;
    }

    //space key - Start / Pause player
    if(ImGui::IsKeyPressed(32) && !io.WantCaptureMouse){
      GUI_Player* gui_player = node_gui->get_gui_player();
      gui_player->player_pause();
      break;
    }
  }

  //----------------------------
}
void GUI_Control::control_keyboard_translation(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  Viewport_obj* view = cameraManager->get_current_viewport();
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
    if(!io.MouseDown[1] && !io.WantCaptureMouse && cloud != nullptr){
      float transCoef = cloud_trans_speed;

      //Shift speed up
      if(io.KeysDown[340]){
        transCoef = cloud_trans_speed * 5;
      }

      // Z key
      if(io.KeysDown[87]){
        vec3 translation = vec3(transCoef,0,0);
        this->key_translation(translation);
        view->cam_P += translation;
        break;
      }
      // S key
      if(io.KeysDown[83]){
        vec3 translation = vec3(-transCoef,0,0);
        this->key_translation(translation);
        view->cam_P += translation;
        break;
      }
      // D key
      if(io.KeysDown[68]){
        vec3 translation = vec3(0,transCoef,0);
        this->key_translation(translation);
        view->cam_P += translation;
        break;
      }
      // Q key
      if(io.KeysDown[65]){
        vec3 translation = vec3(0,-transCoef,0);
        this->key_translation(translation);
        view->cam_P += translation;
        break;
      }
      // A key
      if(io.KeysDown[81]){
        vec3 translation = vec3(0,0,transCoef);
        this->key_translation(translation);
        view->cam_P += translation;
        break;
      }
      // E key
      if(io.KeysDown[69]){
        vec3 translation = vec3(0,0,-transCoef);
        this->key_translation(translation);
        view->cam_P += translation;
        break;
      }
      // 7 key
      if(io.KeysDown[327]){
        float r = cloud_rotat_degree*M_PI/180;
        vec3 rotation = vec3(0,0,r);
        this->key_rotation(rotation);
        break;
      }
      // 9 key
      if(io.KeysDown[329]){
        float r = cloud_rotat_degree*M_PI/180;
        vec3 rotation = vec3(0,0,-r);
        this->key_rotation(rotation);
        break;
      }

      transCoef = cloud_trans_speed;
    }
  }

  //----------------------------
}
void GUI_Control::control_keyboard_ctrlAction(){
  ImGuiIO io = ImGui::GetIO();
  //----------------------------

  //if ctrl
  if(ImGui::IsKeyPressed(341)){
    //ctrl+s - Save as
    if(ImGui::IsKeyPressed(83)){
      Cloud* cloud = sceneManager->get_selected_cloud();
      pathManager->saving_cloud(cloud);
    }
    //ctrl+w - Open
    if(ImGui::IsKeyPressed(90)){
      pathManager->loading();
    }
  }

  //----------------------------
}
void GUI_Control::control_keyboard_camMove(){
  ImGuiIO io = ImGui::GetIO();
  Viewport_obj* view = cameraManager->get_current_viewport();
  //----------------------------

  if(view->cam_move){
    float delta = 0.00016;
    float cam_speed = view->speed_move * delta;
    float cam_speed_fast = view->speed_move * delta * 5;

    for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
      if(io.MouseDown[1] && !io.WantCaptureMouse){

        //Shift speed up
        if(io.KeysDown[340]){
          cam_speed = cam_speed_fast;
        }

        //Z key or Up key
        if(io.KeysDown[87] || io.KeysDown[265]){
          if(view->view == "top"){
            view->cam_P += view->cam_U * cam_speed;
          }else{
            vec3 old = view->cam_P;
            view->cam_P += view->cam_F * cam_speed;
          }
        }

        //S key or Down key
        if(io.KeysDown[83] || io.KeysDown[264]){
          if(view->view == "top"){
            view->cam_P -= view->cam_U * cam_speed;
          }else{
            view->cam_P -= view->cam_F * cam_speed;
          }
        }

        //Q key or Right key
        if(io.KeysDown[65] || io.KeysDown[263]){
          if(view->mode == "default"){
            view->cam_P -= view->cam_R * cam_speed;
          }else if(view->mode == "arcball"){
            vec2 angle =vec2(-cam_speed/10, 0);
            cameraManager->arcball_viewport_angle(angle);
            cameraManager->compute_cam_arcball();
          }
        }

        //D key or Left key
        if(io.KeysDown[68] || io.KeysDown[262]){
          if(view->mode == "default"){
            view->cam_P += view->cam_R * cam_speed;
          }else if(view->mode == "arcball"){
            vec2 angle =vec2(cam_speed/10, 0);
            cameraManager->arcball_viewport_angle(angle);
            cameraManager->compute_cam_arcball();
          }
        }
      }
    }
  }

  //---------------------------
}

//Specific function
void GUI_Control::key_suppr(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //----------------------------

  if(cloud->boxed == false){
    bool is_selected = selectionManager->mark_supressSelectedPoints_all();

    if(is_selected == false){
      sceneManager->remove_cloud(cloud);
    }
  }else{
    boxingManager->supress_selected_point(cloud);
  }

  //----------------------------
}
void GUI_Control::key_c(){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //----------------------------

  if(!sceneManager->get_is_list_empty()){
    poseManager->make_centering(cloud);
    sceneManager->update_cloud_location(cloud);
    sceneManager->update_cloud_glyph(cloud);
  }

  //----------------------------
}
void GUI_Control::key_translation(vec3 trans){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //----------------------------

  transformManager->make_translation(cloud->subset_selected, trans);
  sceneManager->update_subset_location(cloud->subset_selected);
  sceneManager->update_cloud_glyph(cloud);

  //----------------------------
}
void GUI_Control::key_rotation(vec3 rotat){
  Cloud* cloud = sceneManager->get_selected_cloud();
  //----------------------------

  transformManager->make_rotation(cloud->subset_selected, vec3(0,0,0), rotat);
  sceneManager->update_subset_location(cloud->subset_selected);
  sceneManager->update_cloud_glyph(cloud);

  //----------------------------
}
