#ifndef CLOUD_MANAGER_H
#define CLOUD_MANAGER_H

#include "../../common.h"

class GPU_data;


class CloudManager
{
public:
  //Constructor / Destructor
  CloudManager();
  ~CloudManager();

public:


private:
  GPU_data* gpuManager;
};

#endif
