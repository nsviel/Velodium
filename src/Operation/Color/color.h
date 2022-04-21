#ifndef COLOR_H
#define COLOR_H

#include <map>
#include <string>
#include <vector>


//Generic colors
#define color_a1 "#2e4b85"
#define color_a2 "#006997"
#define color_a3 "#00828f"
#define color_a4 "#149676"

//AI class colors
static std::map<std::string, glm::vec4> AI_color_dic = {
  { "Car", vec4(1.0f, 0.0f, 0.0f, 1.0f) },
  { "Cyclist", vec4(0.0f, 1.0f, 0.0f, 1.0f) },
  { "Truck", vec4(1.0f, 1.0f, 0.0f, 1.0f) },
  { "Bus", vec4(1.0f, 0.0f, 1.0f, 1.0f) },
  { "Pedestrian", vec4(0.0f, 0.0f, 1.0f, 1.0f) },
  { "Default", vec4(0.0f, 0.0f, 0.0f, 1.0f) }
};

//Nordic palette
static vec3 nord0 (46,	52, 64); //Dark
static vec3 nord1 (59,	66, 82); //Less dark
static vec3 nord2 (67,	76, 94); //Less less dark
static vec3 nord3 (76,	86, 106); //Less less dark
static vec3 nord4 (216, 222, 233); //Light gray
static vec3 nord5 (229, 233, 240); //More light gray
static vec3 nord6 (236, 239, 244); //More more light gray
static vec3 nord7 (143, 188, 187); //Blue-green
static vec3 nord8 (136, 192, 208); //Cyan
static vec3 nord9 (129, 161, 193); //Light blue
static vec3 nord10 (94, 129, 172); //Blue
static vec3 nord11 (191, 97, 106); //Red
static vec3 nord12 (208, 135,	112); //Orange
static vec3 nord13 (235, 203,	139); //Yellow
static vec3 nord14 (163, 190,	140); //Green
static vec3 nord15 (180, 142,	173); //Purple


#endif
