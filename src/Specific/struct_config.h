#ifndef CONFIG_STRUCT_H
#define CONFIG_STRUCT_H

/**
 * \brief Configuration structure
 * \struct Config struct_config.h "Configuration structure"
 */


#include <string>

struct Config{
  //Windows
  char WINDOW_Title[50];
  float WINDOW_InitResWidth;
  float WINDOW_InitResHeight;
  float WINDOW_Resolution;
  float WINDOW_BckgColor;
  float WINDOW_MultiSample;

  //GUI
  float GUI_LeftPanel_width;
  float GUI_TopPanel_height;
  float GUI_BotPanel_height;
  float GUI_LeftPanel_mid;

  //OpenGL
  bool GL_ForceVersion;
  bool GL_WaitForEvent;
  bool VERBOSE_shader;
  bool VERBOSE_coreGL;

  //Parameters
  char INIT_DefaultDirPath[50];
  float TRANSFORM_Trans;
  float TRANSFORM_TransFast;
  float TRANSFORM_Rot; //Degree
  bool CLOUD_movement;

  //Camera
  float CAM_FOV;
  float CAM_InitialPos;
  float CAM_NearClip;
  float CAM_FarClip;
  float CAM_MouseSpeed;
  float CAM_MoveSpeed;
  float CAM_ZoomSpeed;
};

#endif
