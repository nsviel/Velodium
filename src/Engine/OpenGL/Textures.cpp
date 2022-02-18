#include "Textures.h"


//Constructor / Destructor
Textures::Textures(){
  //---------------------------

  this->list_Texture = new list<Texture*>;

  //---------------------------
}
Textures::~Textures(){}

//Texture management
bool Textures::load_texture(string filePath, string name){
  /*int width = 0;
  int height = 0;
  //---------------------------

  // Load from file
  unsigned char* texData = stbi_load(filePath.c_str(), &width, &height, NULL, 4);
  if(texData == NULL){
    cout<<"Can't load texture file"<<endl;
    return false;
  }

  // Create a OpenGL texture identifier
  GLuint glID;
  glGenTextures(1, &glID);
  glBindTexture(GL_TEXTURE_2D, glID);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);
  stbi_image_free(texData);

  //Create and store texture
  Texture* texture_new = new Texture();
  texture_new->Name = name;
  texture_new->width = width;
  texture_new->height = height;
  texture_new->ID = glID;
  list_Texture->push_back(texture_new);

  //---------------------------*/
  return true;
}
Texture* Textures::get_TextureByName(string name){
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
void Textures::init_skybox(){
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
  uint colorVBO, locationVBO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &locationVBO);
  glBindBuffer(GL_ARRAY_BUFFER, locationVBO);
  glBufferData(GL_ARRAY_BUFFER, XYZ.size() * sizeof(glm::vec3), &XYZ[0], GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(float),(void*)0);
  glEnableVertexAttribArray(0);

  glyph->VAO = VAO;
  glyph->VBO_location = locationVBO;
  glyph->location = XYZ;
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
void Textures::skybox(vec3 camPos){
  /*
  Transformation
  glDepthMask(GL_FALSE);

  glBindVertexArray(glyph->VAO);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
  glDrawArrays(GL_TRIANGLES, 0, glyph->location.size());
  glDepthMask(GL_TRUE);
  */
}
unsigned int Textures::loadCubemap(vector<string> faces){
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
*/
  //---------------------------
  return textureID;
}
