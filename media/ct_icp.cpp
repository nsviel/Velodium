#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdio.h>
#include <math.h>
#include <fstream>
#include <vector>
#include <stdint.h>
#include <random>
#include <unordered_map>
#include <chrono>
#include <fstream>
#include <cstdint>

#include "Eigen/Dense"
#include "Eigen/SVD"
#include "Eigen/Sparse"
#include "Eigen/Geometry"

#include "Utilities/PlyFile.h"


string VELODYNE_REP_IN_BATVISIONSLAM = "C:/Data/BatVision/";
string POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM = "C:/Users/JE/Desktop/SLAM/results_BatVision/";
const int FIRST_INDEX_FRAME_BATVISIONSLAM = 1;
const int LAST_INDEX_FRAME_BATVISIONSLAM_ARRAY[] = {1083, 862, 1541, 1140};

//Parameters Frame LiDAR
const double MIN_DIST_LIDAR_CENTER = 2.0;
const double MAX_DIST_LIDAR_CENTER = 50.0;
const double SIZE_VOXEL = 0.20;

//Parameters Map
const double SIZE_VOXEL_MAP = 0.50;
const int MAX_NUMBER_POINTS_IN_VOXEL = 30;
const double MAX_DIST_MAP = MAX_DIST_LIDAR_CENTER + 0.0;
const double MIN_DIST_BETWEEN_POINTS = 0.10;
const int MIN_NUMBER_NEIGHBORS = 30;
const int MAX_NUMBER_NEIGHBORS = 30;

//Parameters CT ICP
const double SIZE_VOXEL_KEYPOINTS = 0.30;
const int NUMBER_ITER_CT_ICP = 30;
const double MAX_DIST_TO_PLANE_CT_ICP = 0.50;
const double NORM_X_END_ITERATION_CT_ICP = 0.001;

//Optimization
const double LAMBDA_LM = 0.0;

// Traj constraints
const double ALPHA_C = 0.001; //continuity constraint
const double ALPHA_E = 0.001; //ego constraint
const int FREQUENCY_SAVE = 100;
const bool WRITE_POINT_CLOUD = true;

bool is_file_exist(string fileName){
	std::ifstream infile(fileName);
	return infile.good();
}
namespace Eigen{
	typedef Matrix<double, 6, 1> Vector6d;
	typedef Matrix<double, 6, 6> Matrix6d;
}
struct Point3D{
	Eigen::Vector3d  raw_pt;
	Eigen::Vector3d  pt;
	double alpha_timestamp;
	int index_frame;
};
struct TrajectoryFrame{
	Eigen::Matrix3d begin_R;
	Eigen::Vector3d begin_t;
	Eigen::Matrix3d end_R;
	Eigen::Vector3d end_t;
	double timestamp;
};
struct Voxel{
	Voxel(short x, short y, short z) : x(x), y(y), z(z) {}

	bool operator==(const Voxel& vox) const { return x == vox.x && y == vox.y && z == vox.z; }
	bool operator<(const Voxel& vox) const { return x < vox.x || (x == vox.x && y < vox.y) || (x == vox.x && y == vox.y && z < vox.z); }

	short x;
	short y;
	short z;
};
namespace std {
	// Specialization of std::hash for our custom type Voxel
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

void writePoses(string filename, std::vector<TrajectoryFrame> trajectory) {
	ofstream pFile(filename);
	if (pFile.is_open()){
		pFile << "#R(0,0) R(0,1) R(0,2) t(0) R(1,0) R(1,1) R(1,2) t(1) R(2,0) R(2,1) R(2,2) t(2) timestamp" << std::endl;
		pFile << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
		pFile << trajectory[0].begin_R(0, 0) << " " << trajectory[0].begin_R(0, 1) << " " << trajectory[0].begin_R(0, 2) << " " << trajectory[0].begin_t(0) << " " << trajectory[0].begin_R(1, 0) << " " << trajectory[0].begin_R(1, 1) << " " << trajectory[0].begin_R(1, 2) << " " << trajectory[0].begin_t(1) << " " << trajectory[0].begin_R(2, 0) << " " << trajectory[0].begin_R(2, 1) << " " << trajectory[0].begin_R(2, 2) << " " << trajectory[0].begin_t(2) << " " << trajectory[0].timestamp << std::endl;
		for (int i = 0; i < ((int)trajectory.size()-1); i++) {
			Eigen::Quaterniond q_begin = Eigen::Quaterniond(trajectory[i].end_R);
			Eigen::Quaterniond q_end = Eigen::Quaterniond(trajectory[i+1].begin_R);
			Eigen::Vector3d t_begin = trajectory[i].end_t;
			Eigen::Vector3d t_end = trajectory[i+1].begin_t;
			Eigen::Quaterniond q = q_begin.slerp(0.5, q_end);
			q.normalize();
			Eigen::Matrix3d R = q.toRotationMatrix();
			Eigen::Vector3d t = 0.5 * t_begin + 0.5 * t_end;
			pFile << R(0, 0) << " " << R(0, 1) << " " << R(0, 2) << " " << t(0) << " " << R(1, 0) << " " << R(1, 1) << " " << R(1, 2) << " " << t(1) << " " << R(2, 0) << " " << R(2, 1) << " " << R(2, 2) << " " << t(2) << " " << trajectory[i+1].timestamp << std::endl;
		}
		pFile.close();
		return;
	}
	else std::cout << "Unable to open file" << std::endl;
	return;
}
void sub_sample_frame(std::vector<Point3D>& frame, double size_voxel) {
	// Subsample to keep one random point in every voxel of the current frame
	std::unordered_map<Voxel, std::vector<Point3D>> grid;
	for (int i = 0; i < (int)frame.size(); i++) {
		short kx = static_cast<short>(frame[i].pt[0] / size_voxel);
		short ky = static_cast<short>(frame[i].pt[1] / size_voxel);
		short kz = static_cast<short>(frame[i].pt[2] / size_voxel);
		grid[Voxel(kx, ky, kz)].push_back(frame[i]);
	}
	frame.resize(0);
	int step = 0; //to take one random point inside each voxel (but with identical results when lunching the SLAM a second time)
	for (const auto& n : grid) {
		if (n.second.size() > 0) {
			//frame.push_back(n.second[step % (int)n.second.size()]);
			frame.push_back(n.second[0]);
			step++;
		}
	}
}
int ct_icp(std::unordered_map<Voxel, std::list<Eigen::Vector3d>> &voxels_map, std::list<Point3D> &keypoints, std::vector<TrajectoryFrame> &trajectory, int index_frame) {

	short nb_voxels_visited = 0;
	if (index_frame < 100) {
		nb_voxels_visited = 2;
	}
	else {
		nb_voxels_visited = 1;
	}

	double max_dist_to_plane = MAX_DIST_TO_PLANE_CT_ICP;
	int number_keypoints_used = 0;
	double cond_number_A_after = 0.0;

	for (int iter(0); iter < NUMBER_ITER_CT_ICP; iter++) {

		Eigen::MatrixXd A(12, 12);
		Eigen::VectorXd b(12);
		for (int i(0); i < 12; i++) {
			for (int j(0); j < 12; j++) {
				A(i, j) = 0.0;
			}
			b(i) = 0.0;
		}

		number_keypoints_used = 0;
		double total_scalar = 0;
		double mean_scalar = 0.0;

		for (std::list<Point3D>::iterator it_keypoint = keypoints.begin(); it_keypoint != keypoints.end(); ++it_keypoint) {


			Eigen::Vector3d pt_keypoint = (*it_keypoint).pt;

			short kx = static_cast<short>(pt_keypoint[0] / SIZE_VOXEL_MAP);
			short ky = static_cast<short>(pt_keypoint[1] / SIZE_VOXEL_MAP);
			short kz = static_cast<short>(pt_keypoint[2] / SIZE_VOXEL_MAP);

			std::list<std::list<Eigen::Vector3d>*> neighbors_ptr;
			int number_neighbors = 0;
			for (short kxx = kx - nb_voxels_visited; kxx < kx + nb_voxels_visited + 1; ++kxx) {
				for (short kyy = ky - nb_voxels_visited; kyy < ky + nb_voxels_visited + 1; ++kyy) {
					for (short kzz = kz - nb_voxels_visited; kzz < kz + nb_voxels_visited + 1; ++kzz) {
						auto search = voxels_map.find(Voxel(kxx, kyy, kzz));
						if (search != voxels_map.end()) {
							neighbors_ptr.push_back(&(search->second));
							number_neighbors += (int)(search->second).size();
						}
					}
				}
			}

			if (number_neighbors > MIN_NUMBER_NEIGHBORS) {

				std::vector<std::pair<double, Eigen::Vector3d>> vector_neighbors;
				for (std::list<std::list<Eigen::Vector3d>*>::iterator it_ptr = neighbors_ptr.begin(); it_ptr != neighbors_ptr.end(); ++it_ptr) {
					for (std::list<Eigen::Vector3d>::iterator it = (*it_ptr)->begin(); it != (*it_ptr)->end(); ++it) {
						double sq_dist = (pt_keypoint - (*it)).squaredNorm();
						vector_neighbors.push_back(std::make_pair(sq_dist, (*it)));
					}
				}

				std::sort(vector_neighbors.begin(), vector_neighbors.end(), [](const std::pair<double, Eigen::Vector3d> &left, const std::pair<double, Eigen::Vector3d> &right) {return left.first < right.first; });

				int real_number_neighbors = min(MAX_NUMBER_NEIGHBORS, (int)vector_neighbors.size());
				vector_neighbors.resize(real_number_neighbors);

				Eigen::Vector3d barycenter(Eigen::Vector3d(0, 0, 0));
				for (int j = 0; j < (int)vector_neighbors.size(); ++j) {
					barycenter += vector_neighbors[j].second;
				}
				barycenter /= real_number_neighbors;

				Eigen::Matrix3d covariance_Matrix(Eigen::Matrix3d::Zero());
				for (int j = 0; j < (int)vector_neighbors.size(); ++j) {
					for (int k = 0; k < 3; ++k)
						for (int l = k; l < 3; ++l)
							covariance_Matrix(k, l) += (vector_neighbors[j].second(k) - barycenter(k))*(vector_neighbors[j].second(l) - barycenter(l));
				}
				covariance_Matrix(1, 0) = covariance_Matrix(0, 1);
				covariance_Matrix(2, 0) = covariance_Matrix(0, 2);
				covariance_Matrix(2, 1) = covariance_Matrix(1, 2);
				Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(covariance_Matrix);
				Eigen::Vector3d normal(es.eigenvectors().col(0).normalized());

				if (normal.dot(trajectory[index_frame].begin_t - pt_keypoint) < 0) {
					normal = -1.0 * normal;
				}

				double sigma_1 = sqrt(es.eigenvalues()[2]); //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
				double sigma_2 = sqrt(es.eigenvalues()[1]);
				double sigma_3 = sqrt(es.eigenvalues()[0]);
				double a2D = (sigma_2 - sigma_3) / sigma_1;

				//double lambda_1 = es.eigenvalues()[2]; //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
				//double lambda_2 = es.eigenvalues()[1];
				//double lambda_3 = es.eigenvalues()[0];
				//double a2D = (lambda_2 - lambda_3) / (lambda_1);

				double alpha_timestamp = (*it_keypoint).alpha_timestamp;

				double weight = a2D*a2D;
				Eigen::Vector3d closest_pt_normal = weight * normal;

				// Barycenter point
				//double dist_to_plane = normal[0] * (pt_keypoint[0] - barycenter[0]) + normal[1] * (pt_keypoint[1] - barycenter[1]) + normal[2] * (pt_keypoint[2] - barycenter[2]);
				//Eigen::Vector3d closest_point = pt_keypoint - dist_to_plane * normal;

				// Closest Point
				Eigen::Vector3d closest_point = vector_neighbors[0].second;
				double dist_to_plane = normal[0] * (pt_keypoint[0] - closest_point[0]) + normal[1] * (pt_keypoint[1] - closest_point[1]) + normal[2] * (pt_keypoint[2] - closest_point[2]);


				if (fabs(dist_to_plane) < max_dist_to_plane) {

					double scalar = closest_pt_normal[0] * (pt_keypoint[0] - closest_point[0]) + closest_pt_normal[1] * (pt_keypoint[1] - closest_point[1]) + closest_pt_normal[2] * (pt_keypoint[2] - closest_point[2]);
					total_scalar = total_scalar + scalar * scalar;
					mean_scalar = mean_scalar + fabs(scalar);
					number_keypoints_used++;


					Eigen::Vector3d frame_idx_previous_origin_begin = trajectory[index_frame].begin_R * (*it_keypoint).raw_pt;
					Eigen::Vector3d frame_idx_previous_origin_end = trajectory[index_frame].end_R * (*it_keypoint).raw_pt;

					double cbx = (1 - alpha_timestamp) * (frame_idx_previous_origin_begin[1] * closest_pt_normal[2] - frame_idx_previous_origin_begin[2] * closest_pt_normal[1]);
					double cby = (1 - alpha_timestamp) * (frame_idx_previous_origin_begin[2] * closest_pt_normal[0] - frame_idx_previous_origin_begin[0] * closest_pt_normal[2]);
					double cbz = (1 - alpha_timestamp) * (frame_idx_previous_origin_begin[0] * closest_pt_normal[1] - frame_idx_previous_origin_begin[1] * closest_pt_normal[0]);

					double nbx = (1 - alpha_timestamp) * closest_pt_normal[0];
					double nby = (1 - alpha_timestamp) * closest_pt_normal[1];
					double nbz = (1 - alpha_timestamp) * closest_pt_normal[2];

					double cex = (alpha_timestamp) * (frame_idx_previous_origin_end[1] * closest_pt_normal[2] - frame_idx_previous_origin_end[2] * closest_pt_normal[1]);
					double cey = (alpha_timestamp) * (frame_idx_previous_origin_end[2] * closest_pt_normal[0] - frame_idx_previous_origin_end[0] * closest_pt_normal[2]);
					double cez = (alpha_timestamp) * (frame_idx_previous_origin_end[0] * closest_pt_normal[1] - frame_idx_previous_origin_end[1] * closest_pt_normal[0]);

					double nex = (alpha_timestamp)* closest_pt_normal[0];
					double ney = (alpha_timestamp)* closest_pt_normal[1];
					double nez = (alpha_timestamp)* closest_pt_normal[2];

					Eigen::VectorXd u(12);
					u << cbx, cby, cbz, nbx, nby, nbz, cex, cey, cez, nex, ney, nez;
					for (int i = 0; i < 12; i++) {
						for (int j = 0; j < 12; j++) {
							A(i, j) = A(i, j) + u[i] * u[j];
						}
						b(i) = b(i) - u[i] * scalar;
					}
				}
			}
		}
		if (number_keypoints_used < 500) {
			std::cout << "Error : not enough keypoints selected in ct-icp !" << std::endl;
			std::cout << "number_keypoints : " << number_keypoints_used << std::endl;
			system("PAUSE");
		}

		// Normalize equation
		for (int i(0); i < 12; i++) {
			for (int j(0); j < 12; j++) {
				A(i, j) = A(i, j) / number_keypoints_used;
			}
			b(i) = b(i) / number_keypoints_used;
		}

		//Cond number A
		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es_cond;
		es_cond.compute(A);
		double cond_number_A = es_cond.eigenvalues()[11] / es_cond.eigenvalues()[0];

		//Transform Matrix to remove singular elements
		for (int i(0); i < 12; i++) {
			A(i, i) = A(i, i) + LAMBDA_LM;
		}

		//Add constraints in trajectory
		if (index_frame > 1){
			Eigen::Vector3d diff_traj = trajectory[index_frame].begin_t - trajectory[index_frame - 1].end_t;
			A(3,3) = A(3,3) + ALPHA_C;
			A(4,4) = A(4,4) + ALPHA_C;
			A(5,5) = A(5,5) + ALPHA_C;
			b(3) = b(3) - ALPHA_C * diff_traj(0);
			b(4) = b(4) - ALPHA_C * diff_traj(1);
			b(5) = b(5) - ALPHA_C * diff_traj(2);

			Eigen::Vector3d diff_ego = trajectory[index_frame].end_t - trajectory[index_frame].begin_t - trajectory[index_frame - 1].end_t + trajectory[index_frame - 1].begin_t;
			//Eigen::Vector3d diff_ego = trajectory[index_frame].end_t - end_ego;
			A(3, 3) = A(3, 3) + ALPHA_E;
			A(4, 4) = A(4, 4) + ALPHA_E;
			A(5, 5) = A(5, 5) + ALPHA_E;
			A(3, 9) = A(3, 9) - ALPHA_E;
			A(4, 10) = A(4, 10) - ALPHA_E;
			A(5, 11) = A(5, 11) - ALPHA_E;
			A(9, 3) = A(9, 3) - ALPHA_E;
			A(10, 4) = A(10, 4) - ALPHA_E;
			A(11, 5) = A(11, 5) - ALPHA_E;
			A(9, 9) = A(9, 9) + ALPHA_E;
			A(10, 10) = A(10, 10) + ALPHA_E;
			A(11, 11) = A(11, 11) + ALPHA_E;
			b(3) = b(3) + ALPHA_E * diff_ego(0);
			b(4) = b(4) + ALPHA_E * diff_ego(1);
			b(5) = b(5) + ALPHA_E * diff_ego(2);
			b(9) = b(9) - ALPHA_E * diff_ego(0);
			b(10) = b(10) - ALPHA_E * diff_ego(1);
			b(11) = b(11) - ALPHA_E * diff_ego(2);
		}

		//Cond number A
		Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es_cond_after;
		es_cond_after.compute(A);
		cond_number_A_after = es_cond_after.eigenvalues()[2 * 6 - 1] / es_cond_after.eigenvalues()[0];

		//Solve
		Eigen::VectorXd x_bundle = A.ldlt().solve(b);

		double alpha_begin = x_bundle(0);
		double beta_begin = x_bundle(1);
		double gamma_begin = x_bundle(2);
		Eigen::Matrix3d rotation_begin;
		rotation_begin = Eigen::AngleAxisd(alpha_begin, Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(beta_begin, Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(gamma_begin, Eigen::Vector3d::UnitZ());
		Eigen::Vector3d translation_begin = Eigen::Vector3d(x_bundle(3), x_bundle(4), x_bundle(5));

		double alpha_end = x_bundle(6);
		double beta_end = x_bundle(7);
		double gamma_end = x_bundle(8);
		Eigen::Matrix3d rotation_end;
		rotation_end = Eigen::AngleAxisd(alpha_end, Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(beta_end, Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(gamma_end, Eigen::Vector3d::UnitZ());
		Eigen::Vector3d translation_end = Eigen::Vector3d(x_bundle(9), x_bundle(10), x_bundle(11));

		trajectory[index_frame].begin_R = rotation_begin * trajectory[index_frame].begin_R;
		trajectory[index_frame].begin_t = trajectory[index_frame].begin_t + translation_begin;
		trajectory[index_frame].end_R = rotation_end * trajectory[index_frame].end_R;
		trajectory[index_frame].end_t = trajectory[index_frame].end_t + translation_end;

		//Update keypoints
		for (std::list<Point3D>::iterator it_keypoint = keypoints.begin(); it_keypoint != keypoints.end(); ++it_keypoint) {
			Eigen::Quaterniond q_begin = Eigen::Quaterniond(trajectory[index_frame].begin_R);
			Eigen::Quaterniond q_end = Eigen::Quaterniond(trajectory[index_frame].end_R);
			Eigen::Vector3d t_begin = trajectory[index_frame].begin_t;
			Eigen::Vector3d t_end = trajectory[index_frame].end_t;
			double alpha_timestamp = (*it_keypoint).alpha_timestamp;
			Eigen::Quaterniond q = q_begin.slerp(alpha_timestamp, q_end);
			q.normalize();
			Eigen::Matrix3d R = q.toRotationMatrix();
			Eigen::Vector3d t = (1.0 - alpha_timestamp) * t_begin + alpha_timestamp * t_end;
			(*it_keypoint).pt = R * (*it_keypoint).raw_pt + t;
		}

		if (x_bundle.norm() < NORM_X_END_ITERATION_CT_ICP) {
			return number_keypoints_used;
		}
	}

	return number_keypoints_used;
}
void random_sampling(std::vector<Point3D>& frame, std::list<Point3D>& keypoints) {
	keypoints.resize(0);
	int i = 0;
	while (keypoints.size() < 3000) {
		i = i + 100;
		i = i % frame.size();
		keypoints.push_back(frame[i]);
	}
}
void grid_sampling(std::vector<Point3D>& frame, std::list<Point3D>& keypoints, double size_voxel_subsampling) {
	keypoints.resize(0);
	std::vector<Point3D> frame_sub;
	frame_sub.resize(frame.size());
	for (int i = 0; i < (int)frame_sub.size(); i++) {
		frame_sub[i] = frame[i];
	}
	sub_sample_frame(frame_sub, size_voxel_subsampling);
	for (int i = 0; i < (int)frame_sub.size(); i++) {
		keypoints.push_back(frame_sub[i]);
	}
}
void geometric_begin_sampling(std::unordered_map<Voxel, std::list<Eigen::Vector3d>> &voxels_map, std::vector<Point3D>& frame, std::list<Point3D>& keypoints, std::vector<TrajectoryFrame> &trajectory, int index_frame) {
	keypoints.resize(0);
	std::vector<std::vector<std::pair<int, double>>> L(9, std::vector<std::pair<int, double>>());

	Eigen::Vector3d VectorX = trajectory[index_frame].begin_R * Eigen::Vector3d(1., 0., 0.);
	VectorX.normalize();
	Eigen::Vector3d VectorY = trajectory[index_frame].begin_R * Eigen::Vector3d(0., 1., 0.);
	VectorY.normalize();
	Eigen::Vector3d VectorZ = trajectory[index_frame].begin_R * Eigen::Vector3d(0., 0., 1.);
	VectorZ.normalize();

	short nb_voxels_visited = 0;
	if (index_frame < 10) {
		nb_voxels_visited = 2;
	}
	else {
		nb_voxels_visited = 1;
	}

	for (int i = 0; i < (int)frame.size(); i++) {

		short kx = static_cast<short>(frame[i].pt[0] / SIZE_VOXEL_MAP);
		short ky = static_cast<short>(frame[i].pt[1] / SIZE_VOXEL_MAP);
		short kz = static_cast<short>(frame[i].pt[2] / SIZE_VOXEL_MAP);

		std::list<std::list<Eigen::Vector3d>*> neighbors_ptr;
		int number_neighbors = 0;
		for (short kxx = kx - nb_voxels_visited; kxx < kx + nb_voxels_visited + 1; ++kxx) {
			for (short kyy = ky - nb_voxels_visited; kyy < ky + nb_voxels_visited + 1; ++kyy) {
				for (short kzz = kz - nb_voxels_visited; kzz < kz + nb_voxels_visited + 1; ++kzz) {
					auto search = voxels_map.find(Voxel(kxx, kyy, kzz));
					if (search != voxels_map.end()) {
						neighbors_ptr.push_back(&(search->second));
						number_neighbors += (int)(search->second).size();
					}
				}
			}
		}

		if (number_neighbors > MIN_NUMBER_NEIGHBORS) {

			std::vector<std::pair<double, Eigen::Vector3d>> vector_neighbors;
			int j = 0;
			for (std::list<std::list<Eigen::Vector3d>*>::iterator it_ptr = neighbors_ptr.begin(); it_ptr != neighbors_ptr.end(); ++it_ptr) {
				for (std::list<Eigen::Vector3d>::iterator it = (*it_ptr)->begin(); it != (*it_ptr)->end(); ++it) {
					double sq_dist = (frame[i].pt - (*it)).squaredNorm();
					vector_neighbors.push_back(std::make_pair(sq_dist, (*it)));
					j++;
				}
			}

			std::sort(vector_neighbors.begin(), vector_neighbors.end(), [](const std::pair<double, Eigen::Vector3d>& left, const std::pair<double, Eigen::Vector3d>& right) {return left.first < right.first; });

			int real_number_neighbors = min(MAX_NUMBER_NEIGHBORS, (int)vector_neighbors.size());
			vector_neighbors.resize(real_number_neighbors);

			Eigen::Vector3d barycenter(Eigen::Vector3d(0, 0, 0));
			for (int j = 0; j < (int)vector_neighbors.size(); ++j) {
				barycenter += vector_neighbors[j].second;
			}
			barycenter /= real_number_neighbors;

			Eigen::Matrix3d covariance_Matrix(Eigen::Matrix3d::Zero());
			for (int j = 0; j < (int)vector_neighbors.size(); ++j) {
				for (int k = 0; k < 3; ++k)
					for (int l = k; l < 3; ++l)
						covariance_Matrix(k, l) += (vector_neighbors[j].second(k) - barycenter(k)) * (vector_neighbors[j].second(l) - barycenter(l));
			}
			covariance_Matrix(1, 0) = covariance_Matrix(0, 1);
			covariance_Matrix(2, 0) = covariance_Matrix(0, 2);
			covariance_Matrix(2, 1) = covariance_Matrix(1, 2);
			Eigen::SelfAdjointEigenSolver<Eigen::Matrix3d> es(covariance_Matrix);
			Eigen::Vector3d normal(es.eigenvectors().col(0).normalized());

			double sigma_1 = sqrt(es.eigenvalues()[2]); //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
			double sigma_2 = sqrt(es.eigenvalues()[1]);
			double sigma_3 = sqrt(es.eigenvalues()[0]);
			double a2D = (sigma_2 - sigma_3) / sigma_1;

			double d = pow(a2D, 3.0); //tenter avec 1, 2, 3 puis 4
			Eigen::Vector3d c = frame[i].pt.cross(normal);
			L[0].push_back(std::make_pair(i, d * c.dot(VectorX)));
			L[1].push_back(std::make_pair(i, d * c.dot(VectorY)));
			L[2].push_back(std::make_pair(i, d * c.dot(VectorZ)));
			L[3].push_back(std::make_pair(i, d * c.dot(-VectorX)));
			L[4].push_back(std::make_pair(i, d * c.dot(-VectorY)));
			L[5].push_back(std::make_pair(i, d * c.dot(-VectorZ)));

			L[6].push_back(std::make_pair(i, d * fabs(normal.dot(VectorX))));
			L[7].push_back(std::make_pair(i, d * fabs(normal.dot(VectorY))));
			L[8].push_back(std::make_pair(i, d * fabs(normal.dot(VectorZ))));
		}
	}

	for (int i = 0; i < (int)L.size(); i++) {
		std::sort(L[i].begin(), L[i].end(), [](const std::pair<int, double>& left, const std::pair<int, double>& right) {return left.second > right.second; });
	}

	for (int i = 0; i < (int)L.size(); i++) {
		for (int j = 0; j < 300; j++) {
			keypoints.push_back(frame[L[i][j].first]);
		}
	}
}

int main(int argc, char **argv){
	for (int seq = 0; seq <= 3; seq++) {

		stringstream ss_seq;
		ss_seq << setw(2) << setfill('0') << seq;
		string formatedsequence = ss_seq.str();

		//Creating trajectory
		std::vector<TrajectoryFrame> trajectory;
		trajectory.resize(LAST_INDEX_FRAME_BATVISIONSLAM_ARRAY[seq] + 1 - FIRST_INDEX_FRAME_BATVISIONSLAM);

		//Creating voxel_map and keypoint_map
		std::unordered_map<Voxel, std::list<Eigen::Vector3d>> voxels_map;

		double total_diff_traj = 0.0;
		double max_diff_traj = 0.0;
		int index_max_diff_traj = 0;
		int number_diff_traj = 0;

		int mean_number_keypoints = 0;
		int mean_number_keypoints_used = 0;
		int number_frame_keypoints = 0;

		std::vector<Point3D> point_cloud;
		int index_point_cloud = 0;

		for (int index_frame = 0; index_frame < LAST_INDEX_FRAME_BATVISIONSLAM_ARRAY[seq] - FIRST_INDEX_FRAME_BATVISIONSLAM + 1; index_frame++){
			std::vector<Point3D> frame;
			std::vector<Point3D> frame_original;

			double frame_last_timestamp = 0.0;
			double frame_first_timestamp = 1000000000.0;

			//read frame ply
			stringstream ss;
			ss << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
			string formatedFileIndex = ss.str();
			string pointCloudPathIn = VELODYNE_REP_IN_BATVISIONSLAM + formatedsequence + "/frame_" + formatedFileIndex + ".ply";
			while (!is_file_exist(pointCloudPathIn)){
				trajectory[index_frame].begin_R = trajectory[index_frame - 1].begin_R;
				trajectory[index_frame].begin_t = trajectory[index_frame - 1].begin_t;
				trajectory[index_frame].end_R = trajectory[index_frame - 1].end_R;
				trajectory[index_frame].end_t = trajectory[index_frame - 1].end_t;
				index_frame = index_frame + 1;
				stringstream ss2;
				ss2 << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
				formatedFileIndex = ss2.str();
				pointCloudPathIn = VELODYNE_REP_IN_BATVISIONSLAM + formatedsequence + "/frame_" + formatedFileIndex + ".ply";
			}

			PlyFile plyFileIn(pointCloudPathIn, fileOpenMode_IN);
			char* dataIn = nullptr;
			int sizeOfPointsIn = 0;
			int numPointsIn = 0;
			plyFileIn.readFile(dataIn, sizeOfPointsIn, numPointsIn);

			for (int i(0); i < numPointsIn; i++) {

				unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointsIn;
				Point3D new_point;
				new_point.raw_pt[0] = *((float*)(dataIn + offset)); offset += sizeof(float);
				new_point.raw_pt[1] = *((float*)(dataIn + offset)); offset += sizeof(float);
				new_point.raw_pt[2] = *((float*)(dataIn + offset)); offset += sizeof(float);

				new_point.pt = new_point.raw_pt;
				float intensity = *((float*)(dataIn + offset)); offset += sizeof(float);
				new_point.alpha_timestamp = *((float*)(dataIn + offset)); offset += sizeof(float);

				if (new_point.alpha_timestamp < frame_first_timestamp) {
					frame_first_timestamp = new_point.alpha_timestamp;
				}

				if (new_point.alpha_timestamp > frame_last_timestamp) {
					frame_last_timestamp = new_point.alpha_timestamp;
				}

				double r = new_point.raw_pt.norm();
				if ((r > MIN_DIST_LIDAR_CENTER) && (r < MAX_DIST_LIDAR_CENTER)) {
					frame_original.push_back(new_point);
				}
			}

			trajectory[index_frame].timestamp = frame_first_timestamp;
			delete[] dataIn;

			for (int i(0); i < (int)frame_original.size(); i++) {
				frame_original[i].index_frame = index_frame;
				frame_original[i].alpha_timestamp = min(1.0, max(0.0, 1 - (frame_last_timestamp - frame_original[i].alpha_timestamp) / (frame_last_timestamp - frame_first_timestamp)));
			}

			//Copy frame_original to frame
			for (int i = 0; i < (int)frame_original.size(); i++) {
				frame.push_back(frame_original[i]);
			}

			if ((index_frame <= 50) || (index_frame % FREQUENCY_SAVE == 0)) {
				stringstream ss_index_frame;
				ss_index_frame << setw(6) << setfill('0') << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
				string formatedFileIndexFrame = ss_index_frame.str();
				string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/" + formatedFileIndexFrame + ".ply";
				PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
				list<string> properties;
				list<plyTypes> types;
				properties.push_back("x"); types.push_back(float32);
				properties.push_back("y"); types.push_back(float32);
				properties.push_back("z"); types.push_back(float32);
				properties.push_back("scalar"); types.push_back(float32);
				int sizeOfPointOut = 4 * sizeof(float);
				int numPointsOut = (int)frame.size();
				char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
				int i = 0;
				for (int i(0); i < (int)frame.size(); i++) {
					unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
					*((float*)(dataOut + offset)) = (float)frame[i].pt[0]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame[i].pt[1]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame[i].pt[2]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame[i].alpha_timestamp; offset += sizeof(float);
				}
				plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
				delete[] dataOut;
			}

			//Subsample the scan with voxels taking one random in every voxel
			if (index_frame < 50) {
				sub_sample_frame(frame, 0.20);
			}
			else {
				sub_sample_frame(frame, SIZE_VOXEL);
			}

			if ((index_frame <= 50) || (index_frame % FREQUENCY_SAVE == 0)) {
				stringstream ss_index_frame;
				ss_index_frame << setw(6) << setfill('0') << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
				string formatedFileIndexFrame = ss_index_frame.str();
				string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/" + formatedFileIndexFrame + "_sub.ply";
				PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
				list<string> properties;
				list<plyTypes> types;
				properties.push_back("x"); types.push_back(float32);
				properties.push_back("y"); types.push_back(float32);
				properties.push_back("z"); types.push_back(float32);
				properties.push_back("scalar"); types.push_back(float32);
				int sizeOfPointOut = 4 * sizeof(float);
				int numPointsOut = (int)frame.size();
				char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
				int i = 0;
				for (int i(0); i < (int)frame.size(); i++) {
					unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
					*((float*)(dataOut + offset)) = (float)frame[i].pt[0]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame[i].pt[1]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame[i].pt[2]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame[i].alpha_timestamp; offset += sizeof(float);
				}
				plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
				delete[] dataOut;
			}

			// The first frame is static
			if (index_frame == 0) {
				trajectory[index_frame].begin_R = Eigen::MatrixXd::Identity(3, 3);
				trajectory[index_frame].begin_t = Eigen::Vector3d(0., 0., 0.);
				trajectory[index_frame].end_R = Eigen::MatrixXd::Identity(3, 3);
				trajectory[index_frame].end_t = Eigen::Vector3d(0., 0., 0.);
			}
			else {
				if (index_frame == 1) {
					trajectory[index_frame].begin_R = Eigen::MatrixXd::Identity(3, 3);
					trajectory[index_frame].begin_t = Eigen::Vector3d(0., 0., 0.);
					trajectory[index_frame].end_R = Eigen::MatrixXd::Identity(3, 3);
					trajectory[index_frame].end_t = Eigen::Vector3d(0., 0., 0.);
				}
				else {
					//Init guess of the next position of the trajectory

					Eigen::Matrix3d R_next_end = trajectory[index_frame - 1].end_R * trajectory[index_frame - 2].end_R.inverse() * trajectory[index_frame - 1].end_R;
					Eigen::Vector3d t_next_end = trajectory[index_frame - 1].end_t + trajectory[index_frame - 1].end_R * trajectory[index_frame - 2].end_R.inverse() * (trajectory[index_frame - 1].end_t - trajectory[index_frame - 2].end_t);

					trajectory[index_frame].begin_R = trajectory[index_frame - 1].end_R;
					trajectory[index_frame].begin_t = trajectory[index_frame - 1].end_t;

					trajectory[index_frame].end_R = R_next_end;
					trajectory[index_frame].end_t = t_next_end;

					Eigen::Vector3d t_diff = trajectory[index_frame].end_t - trajectory[index_frame].begin_t;

					if (t_diff.norm() > 5.0) {
						std::cout << "Error in ego-motion distance !" << std::endl;
						system("PAUSE");
					}
				}

				Eigen::Quaterniond q_begin = Eigen::Quaterniond(trajectory[index_frame].begin_R);
				Eigen::Quaterniond q_end = Eigen::Quaterniond(trajectory[index_frame].end_R);
				Eigen::Vector3d t_begin = trajectory[index_frame].begin_t;
				Eigen::Vector3d t_end = trajectory[index_frame].end_t;
				for (int i = 0; i < (int)frame.size(); i++) {
					double alpha_timestamp = frame[i].alpha_timestamp;
					Eigen::Quaterniond q = q_begin.slerp(alpha_timestamp, q_end);
					q.normalize();
					Eigen::Matrix3d R = q.toRotationMatrix();
					Eigen::Vector3d t = (1.0 - alpha_timestamp) * t_begin + alpha_timestamp * t_end;
					frame[i].pt = R * frame[i].raw_pt + t;
				}

				if ((index_frame <= 50) || (index_frame % FREQUENCY_SAVE == 0)) {
					stringstream ss_index_frame;
					ss_index_frame << setw(6) << setfill('0') << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
					string formatedFileIndexFrame = ss_index_frame.str();
					string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/" + formatedFileIndexFrame + "_init.ply";
					PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
					list<string> properties;
					list<plyTypes> types;
					properties.push_back("x"); types.push_back(float32);
					properties.push_back("y"); types.push_back(float32);
					properties.push_back("z"); types.push_back(float32);
					int sizeOfPointOut = 3 * sizeof(float);
					int numPointsOut = (int)frame.size();
					char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
					int i = 0;
					for (int i(0); i < (int)frame.size(); i++) {
						unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
						*((float*)(dataOut + offset)) = (float)frame[i].pt[0]; offset += sizeof(float);
						*((float*)(dataOut + offset)) = (float)frame[i].pt[1]; offset += sizeof(float);
						*((float*)(dataOut + offset)) = (float)frame[i].pt[2]; offset += sizeof(float);
					}
					plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
					delete[] dataOut;
				}


				// Use new sub_sample frame as keypoints
				std::list<Point3D> keypoints;
				//random_sampling(frame, keypoints);
				grid_sampling(frame, keypoints, SIZE_VOXEL_KEYPOINTS);
				//geometric_begin_sampling(voxels_map, frame, keypoints, trajectory, index_frame);

				mean_number_keypoints = mean_number_keypoints + (int)keypoints.size();
				number_frame_keypoints++;

				if ((index_frame <= 50) || (index_frame % FREQUENCY_SAVE == 0)) {
					stringstream ss_keypoints;
					ss_keypoints << setw(6) << setfill('0') << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
					string formatedFileIndex_keypoints = ss_keypoints.str();
					string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/" + formatedFileIndex_keypoints + "_keypoints.ply";
					PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
					list<string> properties;
					list<plyTypes> types;
					properties.push_back("x"); types.push_back(float32);
					properties.push_back("y"); types.push_back(float32);
					properties.push_back("z"); types.push_back(float32);
					int sizeOfPointOut = 3 * sizeof(float);
					int numPointsOut = (int)keypoints.size();
					char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
					int i = 0;
					for (std::list<Point3D>::iterator it_pt_key = keypoints.begin(); it_pt_key != keypoints.end(); ++it_pt_key) {
						unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
						*((float*)(dataOut + offset)) = (float)(*it_pt_key).pt[0]; offset += sizeof(float);
						*((float*)(dataOut + offset)) = (float)(*it_pt_key).pt[1]; offset += sizeof(float);
						*((float*)(dataOut + offset)) = (float)(*it_pt_key).pt[2]; offset += sizeof(float);
						i++;
					}
					plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
					delete[] dataOut;
				}

				//Remove voxels too far from actual position of the vehicule
				for (std::unordered_map<Voxel, std::list<Eigen::Vector3d>>::iterator itr_voxel_map = voxels_map.begin(); itr_voxel_map != voxels_map.end(); ++itr_voxel_map) {
					Eigen::Vector3d pt = (*itr_voxel_map).second.front();
					if ((pt - trajectory[index_frame].end_t).squaredNorm() > (MAX_DIST_MAP * MAX_DIST_MAP)) {
						itr_voxel_map = voxels_map.erase(itr_voxel_map);
						//--itr_voxel_map;
					}
				}

				//Number points in the voxels_map
				int numPointsVoxelsMap = 0;
				for (std::unordered_map<Voxel, std::list<Eigen::Vector3d>>::iterator itr_voxel_map = voxels_map.begin(); itr_voxel_map != voxels_map.end(); ++itr_voxel_map) {
					numPointsVoxelsMap += (int)(itr_voxel_map->second).size();
				}

				if ((index_frame <= 50) || (index_frame % FREQUENCY_SAVE == 0)) {
					stringstream ss_map;
					ss_map << setw(6) << setfill('0') << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
					string formatedFileIndex_map = ss_map.str();
					string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/" + formatedFileIndex_map + "_map.ply";
					PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
					list<string> properties;
					list<plyTypes> types;
					properties.push_back("x"); types.push_back(float32);
					properties.push_back("y"); types.push_back(float32);
					properties.push_back("z"); types.push_back(float32);
					int sizeOfPointOut = 3 * sizeof(float);
					int numPointsOut = numPointsVoxelsMap;
					char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
					int i = 0;
					for (std::unordered_map<Voxel, std::list<Eigen::Vector3d>>::iterator itr_voxel_map = voxels_map.begin(); itr_voxel_map != voxels_map.end(); ++itr_voxel_map) {
						for (std::list<Eigen::Vector3d>::iterator it_pt_map = (itr_voxel_map->second).begin(); it_pt_map != (itr_voxel_map->second).end(); ++it_pt_map) {
							unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
							*((float*)(dataOut + offset)) = (float)(*it_pt_map)[0]; offset += sizeof(float);
							*((float*)(dataOut + offset)) = (float)(*it_pt_map)[1]; offset += sizeof(float);
							*((float*)(dataOut + offset)) = (float)(*it_pt_map)[2]; offset += sizeof(float);
							i++;
						}
					}
					plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
					delete[] dataOut;
				}

				int number_keypoints_used = 0;

				//CT-ICP
				number_keypoints_used = ct_icp(voxels_map, keypoints, trajectory, index_frame);
				mean_number_keypoints_used = mean_number_keypoints_used + number_keypoints_used;

				// Compute Modification of trajectory
				double diff_traj = (trajectory[index_frame].begin_t - trajectory[index_frame - 1].end_t).norm();

				if (diff_traj > max_diff_traj) {
					max_diff_traj = diff_traj;
					index_max_diff_traj = index_frame;
				}
				total_diff_traj = total_diff_traj + diff_traj;
				number_diff_traj++;
			}

			//Update frame
			Eigen::Quaterniond q_begin = Eigen::Quaterniond(trajectory[index_frame].begin_R);
			Eigen::Quaterniond q_end = Eigen::Quaterniond(trajectory[index_frame].end_R);
			Eigen::Vector3d t_begin = trajectory[index_frame].begin_t;
			Eigen::Vector3d t_end = trajectory[index_frame].end_t;
			for (int i = 0; i < (int)frame.size(); ++i) {
				double alpha_timestamp = frame[i].alpha_timestamp;
				Eigen::Quaterniond q = q_begin.slerp(alpha_timestamp, q_end);
				q.normalize();
				Eigen::Matrix3d R = q.toRotationMatrix();
				Eigen::Vector3d t = (1.0 - alpha_timestamp) * t_begin + alpha_timestamp * t_end;
				frame[i].pt = R * frame[i].raw_pt + t;
			}

			//Update frame original
			for (int i = 0; i < (int)frame_original.size(); ++i) {
				double alpha_timestamp = frame_original[i].alpha_timestamp;
				Eigen::Quaterniond q = q_begin.slerp(alpha_timestamp, q_end);
				q.normalize();
				Eigen::Matrix3d R = q.toRotationMatrix();
				Eigen::Vector3d t = (1.0 - alpha_timestamp) * t_begin + alpha_timestamp * t_end;
				frame_original[i].pt = R * frame_original[i].raw_pt + t;
			}

			//Write final original frame
			if ((index_frame <= 50) || (index_frame % FREQUENCY_SAVE == 0)) {
				stringstream ss_index_frame;
				ss_index_frame << setw(6) << setfill('0') << index_frame + FIRST_INDEX_FRAME_BATVISIONSLAM;
				string formatedFileIndexFrame = ss_index_frame.str();
				string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/" + formatedFileIndexFrame + "_original_final.ply";
				PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
				list<string> properties;
				list<plyTypes> types;
				properties.push_back("x"); types.push_back(float32);
				properties.push_back("y"); types.push_back(float32);
				properties.push_back("z"); types.push_back(float32);
				int sizeOfPointOut = 3 * sizeof(float);
				int numPointsOut = (int)frame_original.size();
				char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
				int i = 0;
				for (int i(0); i < (int)frame_original.size(); i++) {
					unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
					*((float*)(dataOut + offset)) = (float)frame_original[i].pt[0]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame_original[i].pt[1]; offset += sizeof(float);
					*((float*)(dataOut + offset)) = (float)frame_original[i].pt[2]; offset += sizeof(float);
				}
				plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
				delete[] dataOut;
			}

			//Add original frame to point cloud and Write point cloud
			if (WRITE_POINT_CLOUD) {
				for (int i = 0; i < (int)frame_original.size(); ++i) {
					point_cloud.push_back(frame_original[i]);
				}
				if (point_cloud.size() > 10000000) {
					stringstream ss_index_point_cloud;
					ss_index_point_cloud << index_point_cloud;
					string formatedFileIndexPointCloud = ss_index_point_cloud.str();
					string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/point_cloud_" + formatedFileIndexPointCloud + ".ply";
					PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
					list<string> properties;
					list<plyTypes> types;
					properties.push_back("x"); types.push_back(float32);
					properties.push_back("y"); types.push_back(float32);
					properties.push_back("z"); types.push_back(float32);
					int sizeOfPointOut = 3 * sizeof(float);
					int numPointsOut = (int)point_cloud.size();
					char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
					int i = 0;
					for (int i(0); i < (int)point_cloud.size(); i++) {
						unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
						*((float*)(dataOut + offset)) = (float)point_cloud[i].pt[0]; offset += sizeof(float);
						*((float*)(dataOut + offset)) = (float)point_cloud[i].pt[1]; offset += sizeof(float);
						*((float*)(dataOut + offset)) = (float)point_cloud[i].pt[2]; offset += sizeof(float);
					}
					plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
					delete[] dataOut;

					point_cloud.resize(0);
					index_point_cloud++;
				}
			}

			//Update Voxel Map
			for (int i = 0; i < (int)frame.size(); i++) {
				short kx = static_cast<short>(frame[i].pt[0] / SIZE_VOXEL_MAP);
				short ky = static_cast<short>(frame[i].pt[1] / SIZE_VOXEL_MAP);
				short kz = static_cast<short>(frame[i].pt[2] / SIZE_VOXEL_MAP);

				auto search = voxels_map.find(Voxel(kx, ky, kz));

				if (search != voxels_map.end()) {
					std::list<Eigen::Vector3d>* current_list = &(search->second);

					if ((*current_list).size() < MAX_NUMBER_POINTS_IN_VOXEL) {
						double sq_dist_min_to_points = 10 * SIZE_VOXEL_MAP * SIZE_VOXEL_MAP;
						for (std::list<Eigen::Vector3d>::iterator it_pt_voxel_map = current_list->begin(); it_pt_voxel_map != current_list->end(); ++it_pt_voxel_map) {
							double sq_dist = ((*it_pt_voxel_map) - frame[i].pt).squaredNorm();
							if (sq_dist < sq_dist_min_to_points) {
								sq_dist_min_to_points = sq_dist;
							}
						}
						if (sq_dist_min_to_points > (MIN_DIST_BETWEEN_POINTS * MIN_DIST_BETWEEN_POINTS)) {
							(*current_list).push_back(frame[i].pt);
						}
						//(*current_list).push_back(frame[i].pt);
					}
				}
				else {
					voxels_map[Voxel(kx, ky, kz)].push_back(frame[i].pt);
				}
			}
		}

		//Write last point cloud
		if (WRITE_POINT_CLOUD) {
			stringstream ss_index_point_cloud;
			ss_index_point_cloud << index_point_cloud;
			string formatedFileIndexPointCloud = ss_index_point_cloud.str();
			string pointCloudOutPath = POINT_CLOUD_SLAM_RESULTS_REP_BATVISIONSLAM + formatedsequence + "/point_cloud_" + formatedFileIndexPointCloud + ".ply";
			PlyFile plyFileOut(pointCloudOutPath, fileOpenMode_OUT);
			list<string> properties;
			list<plyTypes> types;
			properties.push_back("x"); types.push_back(float32);
			properties.push_back("y"); types.push_back(float32);
			properties.push_back("z"); types.push_back(float32);
			int sizeOfPointOut = 3 * sizeof(float);
			int numPointsOut = (int)point_cloud.size();
			char* dataOut = new char[(unsigned long long int)numPointsOut * (unsigned long long int)sizeOfPointOut];
			int i = 0;
			for (int i(0); i < (int)point_cloud.size(); i++) {
				unsigned long long int offset = (unsigned long long int)i * (unsigned long long int)sizeOfPointOut;
				*((float*)(dataOut + offset)) = (float)point_cloud[i].pt[0]; offset += sizeof(float);
				*((float*)(dataOut + offset)) = (float)point_cloud[i].pt[1]; offset += sizeof(float);
				*((float*)(dataOut + offset)) = (float)point_cloud[i].pt[2]; offset += sizeof(float);
			}
			plyFileOut.writeFile(dataOut, numPointsOut, properties, types);
			delete[] dataOut;

			point_cloud.resize(0);
			index_point_cloud++;
		}

		std::cout << std::endl;
		std::cout << "Sequence : " << seq << std::endl;
		std::cout << "Total Diff traj : " << total_diff_traj / number_diff_traj << std::endl;
		std::cout << "Max Diff Traj " << max_diff_traj << std::endl;
		std::cout << "Index_max_diff_traj : " << index_max_diff_traj + FIRST_INDEX_FRAME_BATVISIONSLAM << std::endl;
		std::cout << "Mean number keypoints used : " << (double)mean_number_keypoints_used / (double)number_frame_keypoints << std::endl;
	}

	std::cout << "Press enter to continue !" << std::endl;
	std::cin.ignore();
	return 0;
}
