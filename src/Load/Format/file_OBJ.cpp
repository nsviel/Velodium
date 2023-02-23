#include "file_OBJ.h"
#include "../../Interface/File/Info.h"

#include <fstream>


//Constructor / Destructor
file_OBJ::file_OBJ(){}
file_OBJ::~file_OBJ(){}

//Main function
Data_file* file_OBJ::Loader(string path){
  Data_file* data = new Data_file();
  //---------------------------

  // Open file and fill path info
  ifstream file(path);
  data->name = get_name_from_path(path);
  data->path = path;

  // Retrieve file data
  vector<Vertex> vertex_vec = get_data_from_file(file);

  // Fill output format with file data
  for(int i=0; i<vertex_vec.size(); i++){
    data->location.push_back(vertex_vec[i].location);
    data->normal.push_back(vertex_vec[i].normal);
    data->texture.push_back(vertex_vec[i].texcoord);
  }
  data->draw_type = "triangle";

  //---------------------------
  return data;
}

//Subfunction
vector<Vertex> file_OBJ::get_data_from_file(istream& file){
  //---------------------------

  //Initiate vectors
  std::vector<Vertex> vertex_vec;
  std::vector<glm::vec4> xyz(1, glm::vec4( 0, 0, 0, 0 ));
  std::vector<glm::vec3> uv(1, glm::vec3( 0, 0, 0 ));
  std::vector<glm::vec3> Nxyz(1, glm::vec3( 0, 0, 0 ));

  //Read file line by line
  std::string line;
  while(std::getline(file, line)){
    std::istringstream line_str(line);
    std::string line_type;
    line_str >> line_type;

    // location
    if(line_type == "v"){
      float x = 0, y = 0, z = 0, w = 1;
      line_str >> x >> y >> z >> w;
      xyz.push_back( glm::vec4( x, y, z, w ) );
    }

    // texture
    if(line_type == "vt"){
      float u = 0, v = 0, w = 0;
      line_str >> u >> v >> w;
      uv.push_back( glm::vec3( u, v, w ) );
    }

    // normal
    if(line_type == "vn"){
      float i = 0, j = 0, k = 0;
      line_str >> i >> j >> k;
      Nxyz.push_back( glm::normalize( glm::vec3( i, j, k ) ) );
    }

    // polygon
    if(line_type == "f"){
      std::vector<Vertex_ref> refs;
      std::string refStr;
      while( line_str >> refStr){
        std::istringstream ref( refStr );
        std::string vStr, vtStr, vnStr;
        std::getline( ref, vStr, '/' );
        std::getline( ref, vtStr, '/' );
        std::getline( ref, vnStr, '/' );
        int v = atoi( vStr.c_str() );
        int vt = atoi( vtStr.c_str() );
        int vn = atoi( vnStr.c_str() );
        v  = (  v >= 0 ?  v : xyz.size() +  v );
        vt = ( vt >= 0 ? vt : uv.size() + vt );
        vn = ( vn >= 0 ? vn : Nxyz.size()   + vn );
        refs.push_back( Vertex_ref( v, vt, vn ) );
      }

      // triangulate, assuming n>3-gons are convex and coplanar
      for( size_t i = 1; i+1 < refs.size(); ++i){
        const Vertex_ref* p[3] = { &refs[0], &refs[i], &refs[i+1] };

        // http://www.opengl.org/wiki/Calculating_a_Surface_Normal
        glm::vec3 U( xyz[ p[1]->v ] - xyz[ p[0]->v ] );
        glm::vec3 V( xyz[ p[2]->v ] - xyz[ p[0]->v ] );
        glm::vec3 faceNormal = glm::normalize( glm::cross( U, V ) );

        for( size_t j = 0; j < 3; ++j){
          Vertex vertex;
          vertex.location = glm::vec3( xyz[ p[j]->v ] );
          vertex.texcoord = glm::vec2( uv[ p[j]->vt ] );
          vertex.normal = ( p[j]->vn != 0 ? Nxyz[ p[j]->vn ] : faceNormal );
          vertex_vec.push_back( vertex );
        }
      }
    }
  }

  //---------------------------
  return vertex_vec;
}
