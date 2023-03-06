#include "GPU_screenshot.h"
#include "GPU_data.h"

#include "../Core/Dimension.h"
#include "../Core/Configuration.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../extern/image/stb_image_write.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
GPU_screenshot::GPU_screenshot(Dimension* dim){
  //---------------------------

  this->dimManager = dim;
  this->is_screenshot = false;
  this->save_path_screenshot = "";

  //---------------------------
}
GPU_screenshot::~GPU_screenshot(){}

//Screenshot functions
void GPU_screenshot::render_screenshot(string path_file){
  GLFWwindow* window = dimManager->get_window();
  //---------------------------

  if(window != nullptr){
    //First, create image in writing mode
    string path = path_file + "_w";
    vec2 gl_dim = dimManager->get_gl_dim();
    vec2 gl_pos = dimManager->get_gl_pos();

    //Parameters
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_BACK);

    //Configure buffer
    int size = 3 * gl_dim.x * gl_dim.y;
    uint8_t* pixels = new uint8_t[size];
    glReadPixels(gl_pos.x, gl_pos.y, gl_dim.x, gl_dim.y, GL_BGR, GL_UNSIGNED_BYTE, pixels);

    //Freeimage
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, gl_dim.x, gl_dim.y, 3 * gl_dim.x, 24, 0x00FF00 , 0xFF0000 , 0x0000FF, false);
    FreeImage_Save(FIF_BMP, image, path.c_str(), 0);
    FreeImage_Unload(image);

    //Then copie created image into a ready-to-be-read image
    remove(path_file.c_str());
    std::rename(path.c_str(), path_file.c_str());
    //std::filesystem::copy(path, path_file, std::filesystem::copy_options::update_existing);

    //Delete raw data
    delete pixels;
  }

  //---------------------------
}
void GPU_screenshot::render_screenshot_stb_image(string path){
  GLFWwindow* window = dimManager->get_window();
  //---------------------------

  if(window != nullptr){
    vec2 gl_dim = dimManager->get_gl_dim();
    vec2 gl_pos = dimManager->get_gl_pos();

    //Parameters
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);

    //Configure buffer
    GLsizei nrChannels = 3;
    GLsizei stride = nrChannels * gl_dim.x;
    stride += (stride % 4) ? (4 - stride % 4) : 0;
    GLsizei size = stride * gl_dim.y;
    uint8_t* pixels = new uint8_t[size];
    glReadPixels(gl_pos.x, gl_pos.y, gl_dim.x, gl_dim.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    //Reverse red and blue colors
    for(int i=0; i<size; i=i+3){
      uint8_t& a = pixels[i];
      uint8_t& c = pixels[i+2];
      uint8_t b = a;
      a = c;
      c = b;
    }

    //Freeimage
    stbi_write_bmp(path.c_str(), gl_dim.x, gl_dim.y, 3, pixels);

    //Delete raw data
    delete pixels;
  }

  //---------------------------
}
void GPU_screenshot::render_screenshot_pbo(string path){
  GLFWwindow* window = dimManager->get_window();
  static bool first_screenshot = true;
  //---------------------------

  if(window != nullptr){
    if(first_screenshot){
      glGenBuffers(1 , &pbo_screenshot);
      first_screenshot = false;
    }

    vec2 gl_dim = dimManager->get_gl_dim();
    vec2 gl_pos = dimManager->get_gl_pos();

    int pbo_size = gl_dim.x * gl_dim.y * 3;
    uint8_t* pixels = new uint8_t[pbo_size];

    glReadBuffer(GL_BACK);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo_screenshot);
    glReadPixels(0, 0, gl_dim.x, gl_dim.y, GL_RGB, GL_UNSIGNED_BYTE, 0);
    void*buffer_ptr = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, pbo_size, GL_MAP_READ_BIT);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    memcpy(pixels, buffer_ptr, pbo_size);

    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, gl_dim.x, gl_dim.y, 0, 24, 0x0000FF, 0x00FF00, 0xFF0000, false);
    FreeImage_Save(FIF_JPEG, image, path.c_str(), 0);
    FreeImage_Unload(image);

    //Delete raw data
    delete pixels;
  }

  //---------------------------
}
void GPU_screenshot::render_screenshot_freeimage(string path){
  GLFWwindow* window = dimManager->get_window();
  //---------------------------

  if(window != nullptr){
    vec2 gl_dim = dimManager->get_gl_dim();
    vec2 gl_pos = dimManager->get_gl_pos();

    //Parameters
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);

    //Configure buffer
    int size = 3 * gl_dim.x * gl_dim.y;
    uint8_t* pixels = new uint8_t[size];
    glReadPixels(gl_pos.x, gl_pos.y, gl_dim.x, gl_dim.y, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    //Freeimage
    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, gl_dim.x, gl_dim.y, 3 * gl_dim.x, 24, 0xFF0000, 0x00FF00,0x0000FF, false);
    FreeImage_Save(FIF_BMP, image, path.c_str(), 0);
    FreeImage_Unload(image);

    //Delete raw data
    delete pixels;
  }

  //---------------------------
}
void GPU_screenshot::render_screenshot_online(){
  if(is_screenshot && save_path_screenshot != ""){
    auto t1 = std::chrono::high_resolution_clock::now();
    //---------------------------

    this->render_screenshot(save_path_screenshot);
    this->is_screenshot = false;

    //---------------------------
    auto t2 = std::chrono::high_resolution_clock::now();
    this->time_screenshot = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  }
}
