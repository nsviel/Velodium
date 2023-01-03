#include "Registration.h"

#include "Pipeline/ICP.h"
#include "Pipeline/ICP_Matching.h"

#include "../../../Engine/Node_engine.h"
#include "../../../Engine/Scene/Glyph/Glyphs.h"
#include "../../../Engine/Scene/Scene.h"
#include "../../../Operation/Function/Plotting.h"
#include "../../../Operation/Transformation/Transformation.h"
#include "../../../Operation/Transformation/Attribut.h"


//Constructor / Destructor
Registration::Registration(Node_engine* node){
  //---------------------------

  this->sceneManager = node->get_sceneManager();
  this->glyphManager = node->get_glyphManager();
  //this->attribManager = new Attribut(sceneManager);
  this->icpManager = new ICP(glyphManager);
  this->plotManager = new Plotting();

  this->SSE = 10;
  this->SSE_max = 0.000;
  this->nbIter = 0;
  this->nbIter_max = 2500;
  this->colorMeth = 1;

  //---------------------------
}
Registration::~Registration(){}

//Main functions
void Registration::make_Iteration(){
  Cloud* cloud_P = sceneManager->get_selected_cloud();
  Cloud* cloud_Q = sceneManager->get_cloud_next();
  //------------------------------

  //Cloud coloration
  this->colorization(cloud_P, cloud_Q);

  //compute SSE
  SSE = icpManager->compute_SSE(cloud_P, cloud_Q);

  //If ending conditions are fulfill
  if(nbIter < nbIter_max && SSE > SSE_max){
    //ICP algorithm
    tic();
    icpManager->algo(cloud_P, cloud_Q);
    sceneManager->update_cloud_location(cloud_P);
    duration = toc();

    //ICP results
    nbIter++;
    SSE = icpManager->compute_SSE_groundTruth(cloud_P);
    SSE_mean = icpManager->get_SSE_mean();
    vec_SSE.push_back(SSE);
    vec_iter.push_back(nbIter);
  }

  //------------------------------
  //glyphManager->obj_matching(cloud_P, cloud_Q);
}

//Subfunctions
void Registration::reset(){
  //---------------------------

  this->restart();
  icpManager->reset();

  //---------------------------
}
void Registration::restart(){
  //---------------------------

  this->nbIter = 0;
  this->SSE = 10;
  this->duration = 0;

  vec_SSE.clear();
  vec_iter.clear();
  vec_trans.clear();
  vec_rotat.clear();

  //---------------------------
}
void Registration::print_ICP(){
  if(vec_iter.size() < 2){
    cout<<"No enough data for plotting"<<endl;
    return;
  }
  //------------------------------

  plotManager->plot_Curve_discret(vec_iter, vec_SSE);

  //------------------------------
}
void Registration::colorization(Cloud* cloud_P, Cloud* cloud_Q){
  /*//---------------------------

  switch(colorMeth){
    case 0:{ //None
      if(cloud_P->color.hasData && cloud_Q->color.hasData){
        attribManager->set_colorRGB(cloud_P);
        attribManager->set_colorRGB(cloud_Q);
      }else if(cloud_P->intensity.hasData && cloud_Q->intensity.hasData){
        attribManager->set_colorI(cloud_P);
        attribManager->set_colorI(cloud_Q);
      }
      break;
    }
    case 1:{ //Reg / Green
      attribManager->set_pointCloudColor(cloud_P, vec4(1.0f,0.0f,0.0f,1.0f));
      attribManager->set_pointCloudColor(cloud_Q, vec4(0.0f,1.0f,0.0f,1.0f));
      break;
    }
    case 2:{ //Reg / Green -
      attribManager->set_pointCloudColor(cloud_P, vec4(1.0f,0.0f,0.0f,0.3f));
      attribManager->set_pointCloudColor(cloud_Q, vec4(0.0f,1.0f,0.0f,0.3f));
      break;
    }
    case 3:{ //Transparent
      cloud_P->visibility = false;
      cloud_Q->visibility = false;
      break;
    }
  }

  //---------------------------*/
}
