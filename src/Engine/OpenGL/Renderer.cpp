#include "Renderer.h"

#include "../Core/Dimension.h"
#include "../Scene/Configuration.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../extern/image/stb_image_write.h"

#include <filesystem>
#include <FreeImage.h>
#include <cstdint>

// Framebuffer 1 is for point cloud rendering
// Framebuffer 2 is for EDL rendering


//Constructor / Destructor
Renderer::Renderer(Dimension* dim){
  //---------------------------

  this->dimManager = dim;
  this->configManager = new Configuration();

  float bkg_color = configManager->parse_json_f("window", "background_color");
  this->screen_color = vec4(bkg_color, bkg_color, bkg_color, 1.0f);

  this->is_screenshot = false;
  this->save_path_screenshot = "";

  //---------------------------
}
Renderer::~Renderer(){
  //---------------------------

  delete configManager;

  glDeleteFramebuffers(1, &fbo_1_ID);
  glDeleteFramebuffers(1, &fbo_2_ID);

  //---------------------------
}

//Init
void Renderer::init_create_fbo_MSAA(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  glGenTextures(1, &fbo_MSAA_ID);
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbo_MSAA_ID);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 2, GL_RGB, gl_dim.x, gl_dim.y, GL_TRUE);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbo_MSAA_ID, 0);

  //---------------------------
}
void Renderer::init_create_fbo_1(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Create framebuffer 1
  glGenFramebuffers(1, &fbo_1_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);

  //Create (allocate memory) color texture and bind it to the framebuffer
  glGenTextures(1, &fbo_1_tex_color_ID);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gl_dim.x, gl_dim.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_1_tex_color_ID, 0);

  //Create depth texture and bind it to the framebuffer
  glGenTextures(1, &fbo_1_tex_depth_ID);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_depth_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_1_tex_depth_ID, 0);

  //Debind framebuffer
  glBindTexture(GL_TEXTURE_2D ,0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void Renderer::init_create_fbo_2(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Create framebuffer 2
  glGenTextures(1, &fbo_2_tex_color_ID);
  glGenFramebuffers(1, &fbo_2_ID);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_2_ID);

  //Create color texture and bind it to the framebuffer
  glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, fbo_2_tex_color_ID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 64, GL_RGBA, gl_dim.x, gl_dim.y, false);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, fbo_2_tex_color_ID, 0);

  //Debind framebuffer
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
}
void Renderer::init_create_canvas(){
  //---------------------------

  //Generic quad coordinates and UV
  vector<vec2> canvas_xy;
  canvas_xy.push_back(vec2(-1.0f,  1.0f));
  canvas_xy.push_back(vec2(-1.0f,  -1.0f));
  canvas_xy.push_back(vec2(1.0f,  -1.0f));
  canvas_xy.push_back(vec2(-1.0f,  1.0f));
  canvas_xy.push_back(vec2(1.0f,  -1.0f));
  canvas_xy.push_back(vec2(1.0f,  1.0f));

  vector<vec2> canvas_uv;
  canvas_uv.push_back(vec2(0.0f,  1.0f));
  canvas_uv.push_back(vec2(0.0f,  0.0f));
  canvas_uv.push_back(vec2(1.0f,  0.0f));
  canvas_uv.push_back(vec2(0.0f,  1.0f));
  canvas_uv.push_back(vec2(1.0f,  0.0f));
  canvas_uv.push_back(vec2(1.0f,  1.0f));

  //Insert canvas vertices and uv inside gpu
  glGenVertexArrays(1, &canvas_vao);
  glBindVertexArray(canvas_vao);

  glGenBuffers(1, &canvas_vbo_xy);
  glBindBuffer(GL_ARRAY_BUFFER, canvas_vbo_xy);
  glBufferData(GL_ARRAY_BUFFER, canvas_xy.size()*sizeof(glm::vec2), &canvas_xy[0], GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(0);

  GLuint canvas_vbo_uv;
  glGenBuffers(1, &canvas_vbo_uv);
  glBindBuffer(GL_ARRAY_BUFFER, canvas_vbo_uv);
  glBufferData(GL_ARRAY_BUFFER, canvas_uv.size()*sizeof(glm::vec2), &canvas_uv[0], GL_STATIC_DRAW);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);
  glEnableVertexAttribArray(2);

  //---------------------------
}

//Render
void Renderer::bind_fbo_screen(){
  //---------------------------

  //Clear framebuffer and enable depth
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  //---------------------------
}
void Renderer::bind_fbo_render(){
  //---------------------------

  //Bind fbo 2
  glBindFramebuffer(GL_FRAMEBUFFER, fbo_1_ID);
  glDisable(GL_DEPTH_TEST);

  //Bind color and depth textures from fbo 1
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_depth_ID);

  //---------------------------
}
void Renderer::bind_canvas(){
  //---------------------------

  //Bind fbo and clear old one
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClearColor(screen_color.x, screen_color.y, screen_color.z, screen_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  // Current texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);

  //Draw quad
  glBindVertexArray(canvas_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //---------------------------
}

//Update dimensions
void Renderer::update_dim_texture(){
  vec2 gl_dim = dimManager->get_gl_dim();
  //---------------------------

  //Update texture dimensions
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_color_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, gl_dim.x, gl_dim.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
  glBindTexture(GL_TEXTURE_2D, fbo_1_tex_depth_ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, gl_dim.x, gl_dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

  //---------------------------
}
void Renderer::update_dim_canvas(){
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

  vector<vec2> canvas_xy;
  canvas_xy.push_back(tl);
  canvas_xy.push_back(bl);
  canvas_xy.push_back(br);

  canvas_xy.push_back(tl);
  canvas_xy.push_back(br);
  canvas_xy.push_back(tr);

  glBindBuffer(GL_ARRAY_BUFFER, canvas_vbo_xy);
  glBufferData(GL_ARRAY_BUFFER, canvas_xy.size() * sizeof(glm::vec2), &canvas_xy[0],  GL_DYNAMIC_DRAW);

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

    //Delete raw data
    delete pixels;
  }

  //---------------------------
}
void Renderer::render_screenshot_pbo(string path){
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

    //Delete raw data
    delete pixels;
  }

  //---------------------------
}
void Renderer::render_screenshot_online(){
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
