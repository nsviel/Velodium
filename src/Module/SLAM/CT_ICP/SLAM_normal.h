#ifndef SLAM_NORMAL_H
#define SLAM_NORMAL_H

#include "../../../common.h"

#include <queue>
#include <map>
#include <tsl/robin_map.h>

using iNN = std::tuple<double, Eigen::Vector3d>;
struct Comparator {
  bool operator()(const iNN &left, const iNN &right) const {return std::get<0>(left) < std::get<0>(right);}
};
using priority_queue_iNN = std::priority_queue<iNN, std::vector<iNN>, Comparator>;
struct Voxel {

    Voxel() = default;

    Voxel(int x, int y, int z) : x(x), y(y), z(z) {}

    bool operator==(const Voxel &vox) const { return x == vox.x && y == vox.y && z == vox.z; }

    inline bool operator<(const Voxel &vox) const {
        return x < vox.x || (x == vox.x && y < vox.y) || (x == vox.x && y == vox.y && z < vox.z);
    }

    inline static Voxel Coordinates(const Eigen::Vector3d &point, double voxel_size) {
        return {int(point.x() / voxel_size),
                int(point.y() / voxel_size),
                int(point.z() / voxel_size)};
    }

    int x;
    int y;
    int z;
};
struct VoxelBlock {

    explicit VoxelBlock(int num_points = 20) : num_points_(num_points) { points.reserve(num_points); }

    vector<Eigen::Vector3d> points;

    bool IsFull() const { return num_points_ == points.size(); }

    void AddPoint(const Eigen::Vector3d &point) {
      if(num_points_ >= points.size())
        points.push_back(point);
    }

    inline int NumPoints() const { return points.size(); }

    inline int Capacity() { return num_points_; }

private:
    int num_points_;
};
typedef tsl::robin_map<string, vector<Eigen::Vector3d>> voxelMap;


class SLAM_normal
{
public:
  //Constructor / Destructor
  SLAM_normal();
  ~SLAM_normal();

public:
  //Main function
  void compute_frameNormal(Frame* frame, voxelMap* map);

private:
  //Sub functions
  vector<Eigen::Vector3d> compute_kNN_search(Eigen::Vector3d &point, voxelMap* map);
  void compute_normal(vector<Eigen::Vector3d>& kNN, int i);
  void compute_normals_reorientToOrigin(Frame* frame);

private:
  vector<Eigen::Vector3d> Nxyz;
  vector<Eigen::Vector3d> NN;
  vector<float> a2D;

  float size_voxelMap;
};


#endif
