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
#include "Operation/MOD_boxing.h"

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

  this->mod_camera = new MOD_camera(node_engine);
  this->mod_shader = new MOD_shader(node_engine);
  this->mod_operation = new MOD_operation(node_ope);
  this->mod_attribut = new MOD_attribut(node_gui);
  this->mod_cloud = new MOD_cloud(node_ope);
  this->mod_loading = new MOD_loading(node_engine);
  this->mod_transformation = new MOD_transformation(node_engine);
  this->mod_filter = new MOD_filter(node_ope);
  this->mod_extraction = new MOD_extraction(node_ope);
  this->mod_boxing = new MOD_boxing(node_ope);

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
  modal_tab.show_boxing = false;

  //---------------------------
}
void GUI_Modal::window_Draw(){
  //---------------------------

  mod_camera->window_camera();
  mod_shader->window_shader();

  mod_loading->window_loading();
  mod_loading->window_saving();

  mod_cloud->mod_cloud_info();
  mod_cloud->mod_cloud_data();

  mod_transformation->design_transformation();
  mod_filter->design_filter();
  mod_extraction->design_extractCloud();

  mod_operation->window_fitting();
  mod_operation->window_selection();

  mod_attribut->window_normal();
  mod_attribut->window_intensity();
  mod_attribut->window_color();

  mod_boxing->design_boxing();

  //---------------------------
}
