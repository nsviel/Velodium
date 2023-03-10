#ifndef NODE_H
#define NODE_H

#include "../../common.h"


class Node
{
public:
  //Constructor / Destructor
  Node();
  ~Node();

  virtual void init(){}

public:
  //Main functions
  virtual void update(){cout<<"Could not update Node base class"<<endl;}
  virtual void runtime(){cout<<"Could not runtime Node base class"<<endl;}
  virtual void reset(){cout<<"Could not reset Node base class"<<endl;}

private:

};

#endif
