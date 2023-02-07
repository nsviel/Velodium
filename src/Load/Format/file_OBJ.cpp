#include "file_OBJ.h"
#include "../../Interface/File/Info.h"

#include <fstream>


//Constructor / Destructor
file_OBJ::file_OBJ(){}
file_OBJ::~file_OBJ(){}

//Main function
Data_file* file_OBJ::Loader(string filePath){
  ifstream infile(filePath);
  Data_file* data = new Data_file();
  data->name = get_name_from_path(filePath);
  data->path = filePath;
  //---------------------------

  vector<Vertex> file = get_data_from_file(infile);

  for(int i=0; i<file.size(); i++){
    data->location.push_back(file[i].position);
    data->normal.push_back(file[i].normal);
  }

  data->draw_type = "triangle";

  //---------------------------
  return data;
}

//Subfunction
vector<Vertex> file_OBJ::get_data_from_file(istream& in){
  std::vector< Vertex > verts;

  std::vector< glm::vec4 > positions( 1, glm::vec4( 0, 0, 0, 0 ) );
  std::vector< glm::vec3 > texcoords( 1, glm::vec3( 0, 0, 0 ) );
  std::vector< glm::vec3 > normals( 1, glm::vec3( 0, 0, 0 ) );
  std::string lineStr;
  while( std::getline( in, lineStr ) ){
    std::istringstream lineSS( lineStr );
    std::string lineType;
    lineSS >> lineType;

    // vertex
    if( lineType == "v" ){
      float x = 0, y = 0, z = 0, w = 1;
      lineSS >> x >> y >> z >> w;
      positions.push_back( glm::vec4( x, y, z, w ) );
    }

    // texture
    if( lineType == "vt" ){
      float u = 0, v = 0, w = 0;
      lineSS >> u >> v >> w;
      texcoords.push_back( glm::vec3( u, v, w ) );
    }

    // normal
    if( lineType == "vn" ){
      float i = 0, j = 0, k = 0;
      lineSS >> i >> j >> k;
      normals.push_back( glm::normalize( glm::vec3( i, j, k ) ) );
    }

    // polygon
    if( lineType == "f" ){
      std::vector< VertRef > refs;
      std::string refStr;
      while( lineSS >> refStr ){
        std::istringstream ref( refStr );
        std::string vStr, vtStr, vnStr;
        std::getline( ref, vStr, '/' );
        std::getline( ref, vtStr, '/' );
        std::getline( ref, vnStr, '/' );
        int v = atoi( vStr.c_str() );
        int vt = atoi( vtStr.c_str() );
        int vn = atoi( vnStr.c_str() );
        v  = (  v >= 0 ?  v : positions.size() +  v );
        vt = ( vt >= 0 ? vt : texcoords.size() + vt );
        vn = ( vn >= 0 ? vn : normals.size()   + vn );
        refs.push_back( VertRef( v, vt, vn ) );
      }

      // triangulate, assuming n>3-gons are convex and coplanar
      for( size_t i = 1; i+1 < refs.size(); ++i ){
        const VertRef* p[3] = { &refs[0], &refs[i], &refs[i+1] };

        // http://www.opengl.org/wiki/Calculating_a_Surface_Normal
        glm::vec3 U( positions[ p[1]->v ] - positions[ p[0]->v ] );
        glm::vec3 V( positions[ p[2]->v ] - positions[ p[0]->v ] );
        glm::vec3 faceNormal = glm::normalize( glm::cross( U, V ) );

        for( size_t j = 0; j < 3; ++j ){
          Vertex vert;
          vert.position = glm::vec3( positions[ p[j]->v ] );
          vert.texcoord = glm::vec2( texcoords[ p[j]->vt ] );
          vert.normal = ( p[j]->vn != 0 ? normals[ p[j]->vn ] : faceNormal );
          verts.push_back( vert );
        }
      }
    }
  }

  return verts;
}
