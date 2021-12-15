#ifndef COMMON_H
#define COMMON_H


//OpenGL / OpenGLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "Engine/Data/struct_cloud.h"

//Structure
#include "Specific/fct_display.h"
#include "Specific/struct_config.h"
extern struct Config configuration;
#include "Specific/struct_consol.h"
extern struct ConsoleApp console;

//Namespaces
using namespace std;
using namespace glm;

#endif
