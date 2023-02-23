#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "image/stb_image.h"


//Constructor / Destructor
Texture::Texture(){
  //---------------------------

  this->list_texture = new list<Texture_obj*>;

  //---------------------------
}
Texture::~Texture(){}

//Texture management
bool Texture::load_texture(string path, string name){
  int tex_w = 0;
  int tex_h = 0;
  int tex_nb_channel = 0;
  //---------------------------

  // Load from file
  unsigned char* tex_data = stbi_load(path.c_str(), &tex_w, &tex_h, &tex_nb_channel, 0);
  if(tex_data == NULL){
    cout<<"Can't load texture file"<<endl;
    return false;
  }

  // Create a OpenGL texture identifier
  //GLuint tex_ID;
  glGenTextures(1, &tex_ID);
  glBindTexture(GL_TEXTURE_2D, tex_ID);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Upload pixels into texture
  //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  if(tex_nb_channel == 3){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);
  }else if(tex_nb_channel == 4){
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex_w, tex_h, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
  }
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(tex_data);

  //Create and store texture
  Texture_obj* texture = new Texture_obj();
  texture->ID = tex_ID;
  texture->name = name;
  texture->width = width;
  texture->height = height;
  texture->nb_channel = tex_nb_channel;
  list_texture->push_back(texture);

  //---------------------------
  return true;
}
void Texture::apply_texture(Subset* subset, string tex_path){
  //---------------------------

  this->load_texture(tex_path, "texture");
  glBindTexture(GL_TEXTURE_2D, tex_ID);


  //---------------------------
}

/*Texture* Texture::get_TextureByName(string name){
  Texture* texture_out;
  //---------------------------

  for (int i=0; i<list_Texture->size(); i++){
    Texture* texture = *next(list_Texture->begin(),i);

    if(texture->Name == name){
      texture_out = texture;
    }
  }

  //---------------------------
  return texture_out;
}

//Skybox stuff
/* WORK IN PROGRESS */
/*void Texture::init_skybox(){
  //---------------------------

  vector<vec3> XYZ = {
    // positions
    vec3(-1.0f,  1.0f, -1.0f),
    vec3(-1.0f, -1.0f, -1.0f),
    vec3( 1.0f, -1.0f, -1.0f),
    vec3( 1.0f, -1.0f, -1.0f),
    vec3( 1.0f,  1.0f, -1.0f),
    vec3(-1.0f,  1.0f, -1.0f),

    vec3(-1.0f, -1.0f,  1.0f),
    vec3(-1.0f, -1.0f, -1.0f),
    vec3(-1.0f,  1.0f, -1.0f),
    vec3(-1.0f,  1.0f, -1.0f),
    vec3(-1.0f,  1.0f,  1.0f),
    vec3(-1.0f, -1.0f,  1.0f),

    vec3( 1.0f, -1.0f, -1.0f),
    vec3( 1.0f, -1.0f,  1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
    vec3( 1.0f,  1.0f, -1.0f),
    vec3( 1.0f, -1.0f, -1.0f),

    vec3(-1.0f, -1.0f,  1.0f),
    vec3(-1.0f,  1.0f,  1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
    vec3( 1.0f, -1.0f,  1.0f),
    vec3(-1.0f, -1.0f,  1.0f),

    vec3(-1.0f,  1.0f, -1.0f),
    vec3( 1.0f,  1.0f, -1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
    vec3( 1.0f,  1.0f,  1.0f),
    vec3(-1.0f,  1.0f,  1.0f),
    vec3(-1.0f,  1.0f, -1.0f),

    vec3(-1.0f, -1.0f, -1.0f),
    vec3(-1.0f, -1.0f,  1.0f),
    vec3( 1.0f, -1.0f, -1.0f),
    vec3( 1.0f, -1.0f, -1.0f),
    vec3(-1.0f, -1.0f,  1.0f),
    vec3( 1.0f, -1.0f,  1.0f)
  };

  glyph = new Glyph();
  unsigned int VAO;
  uint VBO_xyz;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO_xyz);
  glBindBuffer(GL_ARRAY_BUFFER, VBO_xyz);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);

  glyph->VAO = VAO;
  glyph->VBO_xyz = VBO_xyz;
  glyph->xyz = XYZ;
  glyph->name = "skybox";
  glyph->draw_type = "point";
  glyph->draw_width = 1;
  glyph->permanent = true;

  vector<string> textures_faces;
  textures_faces.push_back("../media_bf/skybox/right.jpg");
  textures_faces.push_back("../media_bf/skybox/left.jpg");
  textures_faces.push_back("../media_bf/skybox/top.jpg");
  textures_faces.push_back("../media_bf/skybox/bottom.jpg");
  textures_faces.push_back("../media_bf/skybox/back.jpg");
  textures_faces.push_back("../media_bf/skybox/front.jpg");
  cubemapTexture = loadCubemap(textures_faces);

  //---------------------------
}
void Texture::skybox(vec3 camPos){
  /*
  Transformation
  glDepthMask(GL_FALSE);

  glBindVertexArray(glyph->VAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, glyph->xyz.size());
  glDepthMask(GL_TRUE);

}*/
/*unsigned int Texture::loadCubemap(vector<string> faces){
  unsigned int textureID;
  //---------------------------
/*
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++)
  {
      unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
      if (data)
      {
          glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                       0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
          stbi_image_free(data);
      }
      else
      {
          std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
          stbi_image_free(data);
      }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  //---------------------------
  return textureID;
}*/
