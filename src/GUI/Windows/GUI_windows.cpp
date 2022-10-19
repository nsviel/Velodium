#include "GUI_windows.h"

#include "Window_table.h"
#include "WIN_loading.h"
#include "WIN_cloud.h"
#include "WIN_camera.h"
#include "WIN_shader.h"
#include "WIN_operation.h"
#include "WIN_attribut.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Operation/Node_operation.h"

#include "imgui/imgui.h"

Window_tab window_tab;


//Constructor / Destructor
GUI_windows::GUI_windows(Node_gui* node_gui){
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();
  Node_engine* node_engine = node_gui->get_node_engine();

  this->win_camera = new WIN_camera(node_engine);
  this->win_shader = new WIN_shader(node_engine);
  this->win_operation = new WIN_operation(node_ope);
  this->win_attribut = new WIN_attribut(node_gui);
  this->win_cloud = new WIN_cloud(node_ope);
  this->win_loading = new WIN_loading(node_engine);

  //---------------------------
  this->window_init();
}
GUI_windows::~GUI_windows(){}

//Main functions
void GUI_windows::window_init(){
  //---------------------------

  window_tab.show_loading = false;
  window_tab.show_saving = false;
  window_tab.show_asciiData = false;
  window_tab.show_camera = false;
  window_tab.show_modifyFileInfo = false;
  window_tab.show_transformation = false;
  window_tab.show_extractCloud = false;
  window_tab.show_cutCloud = false;
  window_tab.show_filter = false;
  window_tab.show_normal = false;
  window_tab.show_intensity = false;
  window_tab.show_color = false;
  window_tab.show_selection = false;
  window_tab.show_fitting = false;
  window_tab.show_shader = false;

  //---------------------------
}
void GUI_windows::window_Draw(){
  //---------------------------

  win_camera->window_camera();
  win_shader->window_shader();

  win_loading->window_loading();
  win_loading->window_saving();

  win_cloud->window_cloudInfo();
  win_cloud->window_asciiData();

  win_operation->window_filter();
  win_operation->window_transformation();
  win_operation->window_extractCloud();
  win_operation->window_cutCloud();
  win_operation->window_fitting();
  win_operation->window_selection();

  win_attribut->window_normal();
  win_attribut->window_intensity();
  win_attribut->window_color();

  //---------------------------
}
