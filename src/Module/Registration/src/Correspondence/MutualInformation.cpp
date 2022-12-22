#include "MutualInformation.h"

#include "../../../../Specific/fct_math.h"


//Constructor / Destructor
MutualInformation::MutualInformation(){}
MutualInformation::~MutualInformation(){}

bool MutualInformation::algo(Subset* subset_P, Subset* subset_Q){
  vector<float>& I_P = subset_P->I;
  vector<float>& I_Q = subset_Q->I;
  //---------------------------

  float h1 = entropy(I_P);
  float h2 = entropy(I_Q);
  float h3 = mutualEntropy(I_P, I_Q);

  float MI = h1 + h2 - h3;

  //---------------------------
  return true;
}

float MutualInformation::entropy(vector<float>& Is){
  int size = Is.size();
  float h = 0;
  //---------------------------

  //vector of probability
  vector<float> x = hist(Is);
  vector<float> p;
  for(int i=0; i<x.size(); i++){
    float pi = x[i]/size;
    p.push_back(pi);
  }

  //Vector of entropy
  vector<float> hi;
  for(int i=0; i<p.size(); i++){
    float h = -p[i] * log2(p[i]);
    hi.push_back(h);
  }

  //entropy
  h = fct_sum(hi);

  //---------------------------
  return h;
}
float MutualInformation::mutualEntropy(vector<float>& I1, vector<float>& I2){
  int s1 = I1.size();
  int s2 = I2.size();
  //---------------------------

  //p1
  vector<float> x1 = hist(I1);
  VectorXf p1(s1);
  for(int i=0; i<x1.size(); i++){
    p1(i) = x1[i]/s1;
  }

  //p1
  vector<float> x2 = hist(I2);
  VectorXf p2(s2);
  for(int i=0; i<x2.size(); i++){
    p2(i) = x2[i]/s2;
  }

  //p12
  MatrixXf p12 = p1*p2;
  float h = p12.sum();

  //---------------------------
  return h;
}
vector<float> MutualInformation::hist(vector<float>& Is){
  vector<float> v_out;
  //---------------------------

  for(int i=0; i<Is.size(); i++){

  }

  //---------------------------
  return v_out;
}
