#include "Renderer.h"

#include "../Dimension.h"

#include "../../Scene/Configuration.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../../extern/stb_image_write.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>


//Constructor / Destructor
Renderer::Renderer(Dimension* dim){
  //---------------------------

  this->dimManager = dim;
  this->configManager = new Configuration();

  float bkg_color = configManager->parse_json_f("window", "background_color");
  this->screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

  this->with_fullscreen = true;
  this->is_screenshot = false;
  glGenBuffers(1 , &pbo);

  //---------------------------
}
Renderer::~Renderer(){}

//Init
void Renderer::init_rendering_fbo_1(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Init FBO 1
  glGenFramebuffers(1, &fbo_1_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);

  //Init textures
  glGenTextures(1, &tex_color_ID);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_ID, 0);

  glGenTextures(1, &tex_depth_ID);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tex_depth_ID, 0);

  //Debind objects
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void Renderer::init_rendering_fbo_2(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Init FBO 2
  glGenTextures(1, &tex_edl_ID);
  glGenFramebuffers(1, &fbo_2_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);

  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, tex_edl_ID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 64, GL_RGBA, gl_dim.x, gl_dim.y, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, tex_edl_ID, 0);

  //Debind objects
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void Renderer::init_rendering_quad(){
  //---------------------------

  vector<vec2> quad_xy;
  quad_xy.push_back(vec2(-1.0f,  1.0f));
  quad_xy.push_back(vec2(-1.0f,  -1.0f));
  quad_xy.push_back(vec2(1.0f,  -1.0f));
  quad_xy.push_back(vec2(-1.0f,  1.0f));
  quad_xy.push_back(vec2(1.0f,  -1.0f));
  quad_xy.push_back(vec2(1.0f,  1.0f));

  vector<vec2> quad_uv;
  quad_uv.push_back(vec2(0.0f,  1.0f));
  quad_uv.push_back(vec2(0.0f,  0.0f));
  quad_uv.push_back(vec2(1.0f,  0.0f));
  quad_uv.push_back(vec2(0.0f,  1.0f));
  quad_uv.push_back(vec2(1.0f,  0.0f));
  quad_uv.push_back(vec2(1.0f,  1.0f));

  glGenVertexArrays(1, &quad_vao);
  glBindVertexArray(quad_vao);

  glGenBuffers(1, &quad_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, quad_xy.size()*sizeof(glm::vec2), &quad_xy[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  GLuint quad_vbo_uv;
  glGenBuffers(1, &quad_vbo_uv);
  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_uv);
  glBufferData(GL_ARRAY_BUFFER, quad_uv.size()*sizeof(glm::vec2), &quad_uv[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(2);

  //---------------------------
}

//Render
void Renderer::render_fbo_1(){
  //---------------------------

  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  //---------------------------
}
void Renderer::render_fbo_2(){
  //---------------------------

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);
  glDisable(GL_DEPTH_TEST);

  //Set active textures
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glActiveTexture(GL_TEXTURE0 + 1);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);

  //---------------------------
}
void Renderer::render_quad(){
  //---------------------------

  //Set FBO
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);

  //Draw quad
  glBindVertexArray(quad_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //---------------------------
}

//Screenshot functions
void Renderer::render_screenshot(string path_file){
  GLFWwindow* window = dimManager->get_window();
  //---------------------------

  if(window != nullptr){
    //First, create image in writing mode
    string path = path_file + "_w";
    vec2 gl_dim = dimManager->get_gl_dim();
    vec2 gl_pos = dimManager->get_gl_pos();

    //Parameters
    glPixelStorei(GL_PACK_ALIGNMENT, 4);
    glReadBuffer(GL_FRONT);

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
  }

  //---------------------------
}
void Renderer::render_screenshot_stb_image(string path){
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
  }

  //---------------------------
}
void Renderer::render_screenshot_pbo(string path){
  GLFWwindow* window = dimManager->get_window();
  //---------------------------

  if(window != nullptr){
    vec2 gl_dim = dimManager->get_gl_dim();
    vec2 gl_pos = dimManager->get_gl_pos();

    int pbo_size = gl_dim.x * gl_dim.y * 3;
    uint8_t* pixels = new uint8_t[pbo_size];


    glReadBuffer(GL_BACK);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, pbo);
    glReadPixels(0, 0, gl_dim.x, gl_dim.y, GL_RGB, GL_UNSIGNED_BYTE, 0);
    void*buffer_ptr = glMapBufferRange(GL_PIXEL_PACK_BUFFER, 0, pbo_size, GL_MAP_READ_BIT);
    glUnmapBuffer(GL_PIXEL_PACK_BUFFER);
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

    memcpy(pixels, buffer_ptr, pbo_size);

    FIBITMAP* image = FreeImage_ConvertFromRawBits(pixels, gl_dim.x, gl_dim.y, 0, 24, 0x0000FF, 0x00FF00, 0xFF0000, false);
    FreeImage_Save(FIF_JPEG, image, path.c_str(), 0);
    FreeImage_Unload(image);
  }

  //---------------------------
}
void Renderer::render_screenshot_freeimage(string path){
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
  }

  //---------------------------
}
void Renderer::render_screenshot_online(){
  if(is_screenshot){
    auto t1 = std::chrono::high_resolution_clock::now();
    //---------------------------

    this->render_screenshot(save_path);
    this->is_screenshot = false;

    //---------------------------
    auto t2 = std::chrono::high_resolution_clock::now();
    this->time_screenshot = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  }
}

//Update
void Renderer::update_texture(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Update texture dimensions
  glBindTexture(GL_TEXTURE_2D, tex_color_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, tex_depth_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

  //---------------------------
}
void Renderer::update_quad(){
  //---------------------------

  vec2 gl_pos = dimManager->get_gl_pos();
  vec2 gl_dim = dimManager->get_gl_dim();
  vec2 win_dim = dimManager->get_win_dim();

  vec2 tl, br, tr, bl;
  bl.x = 2 * (gl_pos.x) / (win_dim.x) - 1;
  bl.y = 2 * (gl_pos.y) / (win_dim.y) - 1;

  br.x = 1;
  br.y = 2 * (gl_pos.y) / (win_dim.y) - 1;

  tl.x = 2 * (gl_pos.x) / (win_dim.x) - 1;
  tl.y = 2 * (gl_pos.y + gl_dim.y) / (win_dim.y) - 1;

  tr.x = 1;
  tr.y = 2 * (gl_pos.y + gl_dim.y) / (win_dim.y) - 1;

  vector<vec2> quad_xy;
  quad_xy.push_back(tl);
  quad_xy.push_back(bl);
  quad_xy.push_back(br);

  quad_xy.push_back(tl);
  quad_xy.push_back(br);
  quad_xy.push_back(tr);

  glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
  glBufferData(GL_ARRAY_BUFFER, quad_xy.size() * sizeof(glm::vec2), &quad_xy[0],  GL_DYNAMIC_DRAW);

  //---------------------------
}
