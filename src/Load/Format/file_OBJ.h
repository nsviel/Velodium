#ifndef FILE_OBJ_H
#define FILE_OBJ_H

#include "../../Engine/Data/struct_dataFile.h"
#include "../../common.h"

struct Vertex{
    glm::vec3 position;
    glm::vec2 texcoord;
    glm::vec3 normal;
};
struct VertRef{
    VertRef( int v, int vt, int vn ) : v(v), vt(vt), vn(vn) { }
    int v, vt, vn;
};


class file_OBJ
{
public:
  //Constructor / Destructor
  file_OBJ();
  ~file_OBJ();

public:
  //Main function
  dataFile* Loader(string filePath);

  //Subfunction
  vector<Vertex> get_data_from_file(istream& in);

private:
  //Datatypes
  dataFile* data_out;
};

#endif
