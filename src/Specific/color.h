#ifndef COLOR_H
#define COLOR_H

#include <map>
#include <string>
#include <vector>
#include <glm/glm.hpp>


//Generic colors
#define color_a1 "#2e4b85"
#define color_a2 "#006997"
#define color_a3 "#00828f"
#define color_a4 "#149676"


//AI class colors
static std::map<std::string, glm::vec4> AI_color_dic = {
  { "Car", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  { "Cyclist", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { "Truck", glm::vec4(1.0f, 1.0f, 0.0f, 1.0f) },
  { "Bus", glm::vec4(1.0f, 0.0f, 1.0f, 1.0f) },
  { "Pedestrian", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { "Default", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f) }
};


//Nordic palette
static glm::vec4 nord0 (0.180392, 0.203922, 0.25098, 1); //Dark
static glm::vec4 nord1 (0.231373, 0.258824, 0.321569, 1); //Less dark
static glm::vec4 nord2 (0.262745, 0.298039, 0.368627, 1); //Less less dark
static glm::vec4 nord3 (0.298039, 0.337255, 0.415686, 1); //Less less dark
static glm::vec4 nord4 (0.847059, 0.870588, 0.913725, 1); //Light gray
static glm::vec4 nord5 (0.898039, 0.913725, 0.941176, 1); //More light gray
static glm::vec4 nord6 (0.92549, 0.937255, 0.956863, 1); //More more light gray
static glm::vec4 nord7 (0.560784, 0.737255, 0.733333, 1); //Blue-green
static glm::vec4 nord8 (0.533333, 0.752941, 0.815686, 1); //Cyan
static glm::vec4 nord9 (0.505882, 0.631373, 0.756863, 1); //Light blue
static glm::vec4 nord10 (0.368627, 0.505882, 0.67451, 1); //Blue
static glm::vec4 nord11 (0.74902, 0.380392, 0.415686, 1); //Red
static glm::vec4 nord12 (0.815686, 0.529412, 0.439216, 1); //Orange
static glm::vec4 nord13 (0.921569, 0.796078, 0.545098, 1); //Yellow
static glm::vec4 nord14 (0.639216, 0.745098, 0.54902, 1); //Green
static glm::vec4 nord15 (0.705882, 0.556863, 0.678431, 1); //Purple


#endif
