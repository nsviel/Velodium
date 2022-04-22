void CT_ICP::add_pointsToLocalMap(Frame* frame){
  //---------------------------

  float dist_max = 0.05;

  for(int i=0; i<frame->xyz.size(); i++){
    Eigen::Vector3f point = frame->xyz[i];

    short vx = static_cast<short>(point(0) / size_voxelMap);
    short vy = static_cast<short>(point(1) / size_voxelMap);
    short vz = static_cast<short>(point(2) / size_voxelMap);

    //Search for pre-existing voxel in local map
    string voxel_id = to_string(vx) + " " + to_string(vy) + " " + to_string(vz);
    auto search = map.find(voxel_id);

    if (search != map.end()) {
      vector<Eigen::Vector3f>& voxel_xyz = search->second;

      if (voxel_xyz.size() < voxel_sizeMax) {
        double dist_min = 10 * pow(size_voxelMap, 2);

        //Check
        for (int j=0; j<voxel_xyz.size(); j++) {
          Eigen::Vector3f voxel_point = voxel_xyz[j];
          float dist = fct_distance(voxel_point, point);

          if (dist < dist_min) {
            dist_min = dist;
          }
        }
        if(dist_min > pow(dist_max, 2)){
          voxel_xyz.push_back(point);
        }
      }
    }else{
      map[voxel_id].push_back(point);
    }

  }

  //---------------------------
}
