#ifndef UDP_STRUCT_H
#define UDP_STRUCT_H

#include <glm/glm.hpp>
#include <vector>

//=========================
/**
 * \brief Point cloud structure storing all associated data
 * \struct cloud struct_pointCloud.h "Point cloud structure"
 */
 //=========================

struct udpPacket{
  //Infos
  int ID; //Permanent cloud ID
  int oID; // Order cloud ID

  std::vector<glm::vec3> xyz;
  std::vector<float> I;
  std::vector<float> R;
  std::vector<float> A;
  std::vector<float> t;
};


#endif
