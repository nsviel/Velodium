#ifndef VOXELMAP_STRUCT_H
#define VOXELMAP_STRUCT_H

#include <Eigen/Dense>
#include <glm/glm.hpp>
#include <tsl/robin_map.h>
#include <queue>


//iNN search
using iNN = std::tuple<double, Eigen::Vector3f>;
struct Comparator {bool operator()(const iNN &left, const iNN &right) const {return std::get<0>(left) < std::get<0>(right);}};
using priority_queue_iNN = std::priority_queue<iNN, std::vector<iNN>, Comparator>;

typedef tsl::robin_map<int, std::vector<Eigen::Vector3f>> voxelMap;
typedef tsl::robin_map<int, std::vector<Eigen::Vector4f>> gridMap;
typedef tsl::robin_map<int, std::vector<glm::vec3>> slamMap;

typedef tsl::robin_map<int, std::vector<Eigen::Vector3f>>::iterator it_voxelMap;
#endif
