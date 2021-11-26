#include "Framebuffer.h"

//Constructor / Destructor
Framebuffer::Framebuffer(){
  //---------------------------

  // Génération d'un second FBO
  glGenFramebuffers(1, &ID_fbo);

  //Generate texture
  glGenTextures(1, &ID_texture);
  glBindTexture(GL_TEXTURE_2D, ID_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 500, 500, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ID_texture, 0);


  float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};


  unsigned int quadVBO;
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



  //---------------------------
}
Framebuffer::~Framebuffer(){}

void Framebuffer::letsgo(){
  //---------------------------


  // first pass
  glBindFramebuffer(GL_FRAMEBUFFER, ID_fbo);
  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
  glEnable(GL_DEPTH_TEST);

  //---------------------------
}
void Framebuffer::letsmid(){
  // second pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}
void Framebuffer::letstop(){

glDisable(GL_DEPTH_TEST);
  glBindVertexArray(quadVAO);

  glBindTexture(GL_TEXTURE_2D, ID_texture);
  glDrawArrays(GL_TRIANGLES, 0, 6);

  //glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDeleteFramebuffers(1, &ID_fbo);
}

//FBO functions
void Framebuffer::display(){
  //---------------------------

  //First pass
  glEnable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, ID_texture);
  glActiveTexture(GL_TEXTURE0);
  glBindFramebuffer(GL_FRAMEBUFFER, ID_fbo);
  glViewport(0, 0, 1024, 768);

  cout<<"First pass OK !"<<endl;

  //Second pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width, height);
  glDisable(GL_DEPTH_TEST);
  glBindTexture(GL_TEXTURE_2D, 0);

  cout<<"Second pass OK !"<<endl;

  //---------------------------
}
void Framebuffer::create_FBO(){
  //---------------------------

  // Génération d'un second FBO
  glGenFramebuffers(1, &ID_fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, ID_fbo);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, ID_texture, 0);

  GLenum DrawBuffers[2] = {GL_COLOR_ATTACHMENT0};
  glDrawBuffers(1, DrawBuffers);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  //---------------------------
  cout<<"FBO created ..."<<endl;
}
void Framebuffer::create_Texture(){
  //---------------------------

  glGenTextures(1, &ID_texture);
  glBindTexture(GL_TEXTURE_2D, ID_texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 768, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  //---------------------------
  cout<<"Texture created ..."<<endl;
}
void Framebuffer::create_DepthRenderBuffer(){
  //---------------------------

  glGenRenderbuffers(1, &ID_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, ID_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, ID_depthBuffer);

  //---------------------------
  cout<<"Depth Render Buffer created ..."<<endl;
}
void Framebuffer::create_bckgrndQuad(){
  //---------------------------

  int numberOfVertices =  4;
  int numberOfTriangles = 2;

  //Initialisation du tableau contenant les coordonnées XYZ des sommets dans le monde
  GLfloat* positions = new GLfloat[3 * numberOfVertices];

  //Initialisation du tableau contenant les coordonnées XYZ des sommets dans la texture
  GLfloat* texCoords = new GLfloat[2 * numberOfVertices];

  //Initialisation du tableau contenant indices des triangles
  GLfloat* indexes = new GLfloat[3 * numberOfTriangles];

  // Coin bas-gauche
  positions[0] = -1.0f;
  positions[1] = -1.0f;
  positions[2] = 0.0f;
  texCoords[0] = 0.0f;
  texCoords[1] = 0.0f;

  // Coin haut-gauche
  positions[3] = -1.0f;
  positions[4] = 1.0f;
  positions[5] = 0.0f;
  texCoords[2] = 0.0f;
  texCoords[3] = 1.0f;

  // Coin haut-droit
  positions[6] = 1.0f;
  positions[7] = 1.0f;
  positions[8] = 0.0f;
  texCoords[4] = 1.0f;
  texCoords[5] = 1.0f;

  // Coin bas-droit
  positions[9] = 1.0f;
  positions[10] = -1.0f;
  positions[11] = 0.0f;
  texCoords[6] = 1.0f;
  texCoords[7] = 0.0f;

  // Face triangulaire 1
  indexes[0] = 0;
  indexes[1] = 1;
  indexes[2] = 2;

  // Face triangulaire 2
  indexes[3] = 2;
  indexes[4] = 3;
  indexes[5] = 0;

  //Génération des VBO
  GLuint idOfPositionArray, idOfTexCoordArray, idOfIndexArray;
  glGenBuffers(1, &idOfPositionArray);
  glGenBuffers(1, &idOfTexCoordArray);
  glGenBuffers(1, &idOfIndexArray);

  //Remplissage des VBO
  glBindBuffer(GL_ARRAY_BUFFER, idOfPositionArray);
  glBufferData(GL_ARRAY_BUFFER, 3 * numberOfVertices * sizeof(GLfloat), positions, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, idOfTexCoordArray);
  glBufferData(GL_ARRAY_BUFFER,  2 * numberOfVertices * sizeof(GLfloat), texCoords, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idOfIndexArray);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * numberOfTriangles * sizeof(GLuint), indexes, GL_STATIC_DRAW);

  //---------------------------
  cout<<"Background quad created ..."<<endl;
}

void Framebuffer::readScreenPixels(int width, int height){
  cout<<"Make screenshot ... "<<width<<" "<<height<<endl;
  //---------------------------

  int nPixels= 3 * width * height;
  GLfloat* pixels = new GLfloat[nPixels];
  glReadPixels(0.0, 0.0, width, height, GL_RGB, GL_FLOAT, pixels);

  //---------------------------
  cout<<"Screenshot ok "<<endl;
}
