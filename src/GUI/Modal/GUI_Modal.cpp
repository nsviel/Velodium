#include "GUI_Modal.h"

#include "Modal_tab.h"
#include "Scene/MOD_loading.h"
#include "Scene/MOD_cloud.h"
#include "Scene/MOD_camera.h"
#include "Scene/MOD_shader.h"
#include "Operation/MOD_operation.h"
#include "Operation/MOD_attribut.h"
#include "Operation/MOD_transformation.h"
#include "Operation/MOD_filter.h"
#include "Operation/MOD_extraction.h"

#include "../Node_gui.h"

#include "../../Engine/Node_engine.h"
#include "../../Operation/Node_operation.h"

#include "imgui/imgui.h"

Modal_tab modal_tab;


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
  this->mod_transformation = new MOD_transformation(node_engine);
  this->mod_filter = new MOD_filter(node_ope);
  this->mod_extraction = new MOD_extraction(node_ope);

  //---------------------------
  this->window_init();
}
GUI_Modal::~GUI_Modal(){}

//Main functions
void GUI_Modal::window_init(){
  //---------------------------

  modal_tab.show_loading = false;
  modal_tab.show_saving = false;
  modal_tab.show_asciiData = false;
  modal_tab.show_camera = false;
  modal_tab.show_modifyFileInfo = false;
  modal_tab.show_transformation = false;
  modal_tab.show_extractCloud = false;
  modal_tab.show_filter = false;
  modal_tab.show_normal = false;
  modal_tab.show_intensity = false;
  modal_tab.show_color = false;
  modal_tab.show_selection = false;
  modal_tab.show_fitting = false;
  modal_tab.show_shader = false;

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

  mod_transformation->design_transformation();
  mod_filter->design_filter();
  mod_extraction->design_extractCloud();

  win_operation->window_fitting();
  win_operation->window_selection();

  win_attribut->window_normal();
  win_attribut->window_intensity();
  win_attribut->window_color();

  //---------------------------
}
