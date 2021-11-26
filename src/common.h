#ifndef COMMON_H
#define COMMON_H


//OpenGL / OpenGLM
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

//External
#include "../extern/gnuplot/gnuplot-iostream.h"
#include "../extern/IconsFontAwesome5.h"

//Structure
#include "Specific/fct_display.h"
#include "Specific/struct_config.h"
extern struct Config configuration;
#include "Specific/struct_consol.h"
extern struct ConsoleApp console;
#include "Engine/Data/Database.h"
extern struct Database database;

//Namespaces
using namespace std;
using namespace glm;
using namespace Eigen;

#endif
