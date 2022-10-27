#include "GUI_Modal.h"

#include "Modal_tab.h"
#include "MOD_loading.h"
#include "MOD_cloud.h"
#include "MOD_camera.h"
#include "MOD_shader.h"
#include "MOD_operation.h"
#include "MOD_attribut.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Operation/Node_operation.h"

#include "imgui/imgui.h"

Modal_tab window_tab;


//Constructor / Destructor
GUI_Modal::GUI_Modal(Node_gui* node_gui){
  //---------------------------

  Node_operation* node_ope = node_gui->get_node_ope();
  Node_engine* node_engine = node_gui->get_node_engine();

  this->win_camera = new MOD_camera(node_engine);
  this->win_shader = new MOD_shader(node_engine);
  this->win_operation = new MOD_operation(node_ope);
  this->win_attribut = new MOD_attribut(node_gui);
  this->win_cloud = new MOD_cloud(node_ope);
  this->win_loading = new MOD_loading(node_engine);

  //---------------------------
  this->window_init();
}
GUI_Modal::~GUI_Modal(){}

//Main functions
void GUI_Modal::window_init(){
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
void GUI_Modal::window_Draw(){
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
