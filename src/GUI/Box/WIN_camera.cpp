#include "WIN_camera.h"

#include "IconsFontAwesome5.h"

#include "../../Engine/Node_engine.h"
#include "../../Engine/Scene/Configuration.h"
#include "../../Engine/OpenGL/Camera/Camera.h"

#include <fstream>

#include "Window_table.h"
extern struct Window_tab window_tab;


//Constructor / Destructor
WIN_camera::WIN_camera(Node_engine* node_engine){
  //---------------------------

  this->configManager = node_engine->get_configManager();
  this->cameraManager = node_engine->get_cameraManager();

  this->item_width = 150;

  //---------------------------
}
WIN_camera::~WIN_camera(){}

//Main function
void WIN_camera::window_camera(){
  bool* open = &window_tab.show_camera;
  if(*open){
    ImGui::Begin(ICON_FA_CAMERA " Camera", open,ImGuiWindowFlags_AlwaysAutoResize);
    //---------------------------

    this->cam_parameter();
    this->cam_info();

    //---------------------------
    ImGui::Separator();
    if(ImGui::Button("Close")){
      *open = false;
    }
    ImGui::End();
  }
}

//Sub functions
void WIN_camera::cam_parameter(){
  //---------------------------

  //Zoom - Field Of View
  float fov_value = cameraManager->get_fov();
  if(ImGui::SliderFloat("FOV (°)", &fov_value, 100.0f, 1.0f)){
    cameraManager->set_cameraFOV(fov_value);
  }
  static float cam_speed = configManager->parse_json_f("camera", "speed_move");
  if(ImGui::DragFloat("speed (m/s)", &cam_speed, 0.01, 0, 20, "%.2f")){
    cameraManager->set_cameraSpeed(cam_speed);
  }
  ImGui::Separator();

  //Camera mode
  ImGui::Columns(2);

  static int projection = 0;
  ImGui::Text("Projection");
  if(ImGui::RadioButton("Perspective", &projection, 0)){
    cameraManager->input_set_projection(projection);
  }
  if(ImGui::RadioButton("Orthographic", &projection, 1)){
    cameraManager->input_set_projection(projection);
  }

  ImGui::NextColumn();

  static int view = 1;
  ImGui::Text("View");
  if(ImGui::RadioButton("Top", &view, 0)){
    cameraManager->input_set_view(view);
  }
  if(ImGui::RadioButton("Oblique", &view, 1)){
    cameraManager->input_set_view(view);
  }

  ImGui::Columns(1);

  //---------------------------
  ImGui::Separator();
}
void WIN_camera::cam_info(){
  //---------------------------

  //Camera projection matrix
  glm::mat4 cam_pos = cameraManager->compute_cam_world_pose();
  ImGui::Text("Model-View matrix");
  ImGui::SameLine();
  if(ImGui::Button("Print")){
    sayMat4(cam_pos);
  }
  ImGui::Columns(4, "Proj");
  for(int i=0;i<4;i++){
    ImGui::Separator();
    for(int j=0;j<4;j++){
      ImGui::Text("%.3f", cam_pos[i][j]);
      ImGui::NextColumn();
    }
  }
  ImGui::Separator();
  ImGui::Columns(1);

  //Camera position
  vec3* cam_position = cameraManager->get_camPosPtr();
  float *floatArray = &cam_position[0].x;
  if(ImGui::Button("R")){
    *cam_position = vec3(0,0,0);
  }
  ImGui::SameLine();
  ImGui::DragFloat3("Pos", floatArray, 0.01f, -100.0f, 100.0f);

  //Camera angles
  float* HAngle = cameraManager->get_angle_azimuth();
  float VAngle = cameraManager->get_angle_elevati();
  ImGui::Text("Horizontal angle : %.2f°", *HAngle * 180 / M_PI);
  ImGui::Text("Vertical angle : %.2f°", VAngle * 180 / M_PI);

  //---------------------------
}
void WIN_camera::cam_definedPosition(){
  //---------------------------

  //Insert pre-defined pose
  static bool dPoseInit = false;
  if(ImGui::Button("Insert")){
    string zenity = "zenity --file-selection --title=CameraPose 2> /dev/null";
    FILE *file = popen(zenity.c_str(), "r");
    char filename[1024];
    const char* path_char = fgets(filename, 1024, file);
    mat4 dP;

    //Check if not empty
    if ((path_char != NULL) && (path_char[0] != '\0')){
      string path_str(path_char);
      path_str = path_str.substr(0, path_str.find('\n'));
      std::ifstream infile(path_str);
      std::string line;
      float a, b ,c, d;
      int cpt = 0;

      while (std::getline(infile, line))
      {
        std::istringstream iss(line);
        iss >> a >> b >> c >> d;

        dP[cpt][0] = a;
        dP[cpt][1] = b;
        dP[cpt][2] = c;
        dP[cpt][3] = d;

        cpt++;
      }

      mat4 orientation(
         dP[0][0], dP[0][1], dP[0][2], 0,
         dP[1][0], dP[1][1], dP[1][2], 0,
         dP[2][0], dP[2][1], dP[2][2], 0,
           0,       0,       0,     1);

      mat4 translation(
                1,       0,       0, 0,
                0,       1,       0, 0,
                0,       0,       1, 0,
          -dP[0][3], -dP[1][3], -dP[2][3], 1);

      mat4 mvMatrix = orientation * translation;
      vec3 camPos = vec3(dP[0][3], dP[1][3], dP[2][3]);
      cameraManager->set_cameraPos(camPos);
      cameraManager->set_desiredViewMatrix(mvMatrix);

      dPoseInit = true;
      cameraManager->set_desiredPoseON(dPoseInit);
    }
  }
  ImGui::SameLine();
  static bool blockPose = false;
  if(ImGui::Checkbox("Block pose", &blockPose) && dPoseInit == true){
    cameraManager->set_desiredPoseON(dPoseInit);
  }

  //---------------------------
}
