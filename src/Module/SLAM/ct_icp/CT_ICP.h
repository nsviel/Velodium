#ifndef SLAM_CTICP_H
#define SLAM_CTICP_H

#include "../../../common.h"


struct Point3D{
	Eigen::Vector3d  raw_pt;
	Eigen::Vector3d  pt;
	double timestamp;
	int index_frame;
};
struct TrajectoryFrame{
	Eigen::Matrix3d center_R;
	Eigen::Vector3d center_t;
};
struct Voxel{
	Voxel(short x, short y, short z) : x(x), y(y), z(z) {}

	bool operator==(const Voxel& vox) const { return x == vox.x && y == vox.y && z == vox.z; }
	bool operator<(const Voxel& vox) const { return x < vox.x || (x == vox.x && y < vox.y) || (x == vox.x && y == vox.y && z < vox.z); }

	short x;
	short y;
	short z;
};

using namespace std;
namespace Eigen {
	typedef Matrix<double, 6, 1> Vector6d;
	typedef Matrix<double, 6, 6> Matrix6d;
}
namespace std {
	template <>
	struct hash<Voxel> {
		std::size_t operator()(const Voxel& vox) const {
			std::hash<int32_t> hasher;
			return ((hasher(vox.x)
				^ (hasher(vox.y) << 1)) >> 1)
				^ (hasher(vox.z) << 1) >> 1;
		}
	};
}


class CT_ICP
{
public:
  //Constructor / Destructor
  CT_ICP();
  ~CT_ICP();

public:
  void algo(Collection* collection);
	void algo(Cloud* cloud);
	void reset();

private:
	//Algo functions
	void do_truc(Cloud* cloud, vector<Point3D>& frame);
	void update_voxelMap(vector<Point3D>& frame);

	//Subfunctions
  void writePoses(string filename, std::vector<TrajectoryFrame> trajectory);
  int frame_to_model(std::unordered_map<Voxel, std::list<Eigen::Vector3d>>& voxels_map, std::list<Point3D>& keypoints, std::vector<TrajectoryFrame>& trajectory, int index_frame);
  void sub_sample_frame(std::vector<Point3D>& frame, double size_voxel);
  void grid_sampling(std::vector<Point3D>& frame, std::list<Point3D>& keypoints, double size_voxel_subsampling);

private:
	std::unordered_map<Voxel, std::list<Eigen::Vector3d>> voxels_map;
	std::vector<TrajectoryFrame> trajectory;
	int index_frame;
};


#endif
