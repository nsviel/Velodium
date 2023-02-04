#include "CT_ICP.h"

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

#include "Eigen/Dense"
#include "Eigen/SVD"
#include "Eigen/Sparse"
#include "Eigen/Geometry"

#include "Utilities/PlyFile.h"
#include "Utilities/PersoTimer.h"


string VELODYNE_REP_IN_REP = "C:/Data/5GMed/";
string POINT_CLOUD_SLAM_RESULTS_REP = "C:/Users/JE/Desktop/SLAM/results_5GMed/";
const int FIRST_INDEX_FRAME = 27502;
const int LAST_INDEX_FRAME = 38480;

//Parameters Frame LiDAR
const double MIN_DIST_LIDAR_CENTER = 5.0;
const double MAX_DIST_LIDAR_CENTER = 100.0;
const double SIZE_VOXEL = 0.20;

//Parameters Map
const double SIZE_VOXEL_MAP = 1.5;
const int MAX_NUMBER_POINTS_IN_VOXEL = 30;
const double MAX_DIST_MAP = MAX_DIST_LIDAR_CENTER;
//const double MIN_DIST_BETWEEN_POINTS = 0.20;
const int MIN_NUMBER_NEIGHBORS = 30;
const int MAX_NUMBER_NEIGHBORS = 30;
const int NUMBER_KEYPOINTS = 1000;

//Parameters Frame to Model (just for frame index_frame == 1)
const int NUMBER_ITER_FRAME_TO_MODEL = 8;
const double START_DIST_TO_PLANE_FRAME_TO_MODEL = 1.50;
const double MAX_DIST_TO_PLANE_FRAME_TO_MODEL = 0.50;
const double NORM_X_END_ITERATION_FRAME_TO_MODEL = 0.001;

//Optimization
const double ALPHA_E = 1.0; //constraint on motion model


//Constructor / Destructor
CT_ICP::CT_ICP(){
	//---------------------------

	this->index_frame = 0;

	//---------------------------
}
CT_ICP::~CT_ICP(){}

//Main function
void CT_ICP::algo(Cloud* cloud){
	if(cloud == nullptr) return;
	//---------------------------

	//Creating trajectory
	std::vector<TrajectoryFrame> trajectory;
	trajectory.resize(cloud->nb_subset);

	//Creating voxel_map and keypoint_map
	std::unordered_map<Voxel, std::list<Eigen::Vector3d>> voxels_map;
	int index_frame = 0;

	for(int i=0; i<cloud->nb_subset; i++){
		Subset* subset = *next(cloud->subset.begin(), i);

		// subset to local format
		std::vector<Point3D> frame;
		for(int j=0; j<subset->xyz.size(); j++){
			Point3D new_point;
			new_point.raw_pt[0] = subset->xyz[j].x;
			new_point.raw_pt[1] = subset->xyz[j].y;
			new_point.raw_pt[2] = subset->xyz[j].z;
			new_point.pt = new_point.raw_pt;
			new_point.timestamp = subset->ts[j];
			new_point.index_frame = index_frame;

			double r = new_point.raw_pt.norm();
			if ((r > MIN_DIST_LIDAR_CENTER) && (r < MAX_DIST_LIDAR_CENTER)) {
				frame.push_back(new_point);
			}
		}

		//Subsample the scan with voxels taking one random in every voxel
		this->sub_sample_frame(frame, SIZE_VOXEL);

		// The first frame is static
		if (index_frame == 0) {
			trajectory[index_frame].center_R = Eigen::MatrixXd::Identity(3, 3);
			trajectory[index_frame].center_t = Eigen::Vector3d(0., 0., 0.);
		}
		else {
			if (index_frame == 1) {
				trajectory[index_frame].center_R = Eigen::MatrixXd::Identity(3, 3);
				trajectory[index_frame].center_t = Eigen::Vector3d(0., 0., 0.);
			}
			else {
				//Init guess of the next position of the trajectory
				Eigen::Matrix3d R_next_center = trajectory[index_frame - 1].center_R * trajectory[index_frame - 2].center_R.inverse() * trajectory[index_frame - 1].center_R;
				Eigen::Vector3d t_next_center = trajectory[index_frame - 1].center_t + trajectory[index_frame - 1].center_R * trajectory[index_frame - 2].center_R.inverse() * (trajectory[index_frame - 1].center_t - trajectory[index_frame - 2].center_t);
				trajectory[index_frame].center_R = R_next_center;
				trajectory[index_frame].center_t = t_next_center;

				Eigen::Vector3d t_diff = trajectory[index_frame].center_t - trajectory[index_frame-1].center_t;
				if (t_diff.norm() > 5.0) {
					std::cout << "Error in current motion distance !" << std::endl;
				}
			}

			for (int i = 0; i < (int)frame.size(); ++i) {
				frame[i].pt = trajectory[index_frame].center_R * frame[i].raw_pt + trajectory[index_frame].center_t;
			}

			// Use new sub_sample frame as keypoints
			std::list<Point3D> keypoints;
			keypoints.resize(0);
			this->grid_sampling(frame, keypoints, 2.0);
			if (keypoints.size() < NUMBER_KEYPOINTS) {
				keypoints.resize(0);
				this->grid_sampling(frame, keypoints, 1.0);
				if (keypoints.size() < NUMBER_KEYPOINTS) {
					keypoints.resize(0);
					grid_sampling(frame, keypoints, 0.50);
					if (keypoints.size() < NUMBER_KEYPOINTS) {
						keypoints.resize(0);
						grid_sampling(frame, keypoints, 0.20);
					}
				}
			}
			if (keypoints.size() > NUMBER_KEYPOINTS) {
				keypoints.resize(NUMBER_KEYPOINTS);
			}

			//Remove voxels too far from actual position of the vehicule
			for (std::unordered_map<Voxel, std::list<Eigen::Vector3d>>::iterator it = voxels_map.begin(); it != voxels_map.end(); ++it) {
				Eigen::Vector3d pt = (*it).second.front();
				if ((pt - trajectory[index_frame].center_t).squaredNorm() > (MAX_DIST_MAP * MAX_DIST_MAP)) {
					it = voxels_map.erase(it);
				}
			}

			//Frame To Model
			int number_keypoints_used = frame_to_model(voxels_map, keypoints, trajectory, index_frame);
			if(number_keypoints_used == 0){
				this->reset();
				return;
			}

			//Update frame
			for(int j=0; j<subset->xyz.size(); j++){
				Eigen::Vector3d point;
				point << subset->xyz[j].x, subset->xyz[j].y, subset->xyz[j].z;
				point = trajectory[index_frame].center_R * point + trajectory[index_frame].center_t;
				subset->xyz[j] = vec3(point(0), point(1), point(2));
			}
		}

		//Update Voxel Map
		for (int j=0; j<(int)frame.size(); j++) {
			short kx = static_cast<short>(frame[j].pt[0] / SIZE_VOXEL_MAP);
			short ky = static_cast<short>(frame[j].pt[1] / SIZE_VOXEL_MAP);
			short kz = static_cast<short>(frame[j].pt[2] / SIZE_VOXEL_MAP);

			auto search = voxels_map.find(Voxel(kx, ky, kz));

			if (search != voxels_map.end()) {
				std::list<Eigen::Vector3d>* current_list = &(search->second);

				if ((*current_list).size() < MAX_NUMBER_POINTS_IN_VOXEL) {
					(*current_list).push_back(frame[j].pt);
				}
			}
			else {
				voxels_map[Voxel(kx, ky, kz)].push_back(frame[j].pt);
			}
		}

		index_frame++;
	}

	// Write complete traj in LiDAR reference
	this->writePoses("result_cticp.txt", trajectory);

	//---------------------------
}
void CT_ICP::algo(Subset* subset){
	if(subset == nullptr) return;
	//---------------------------

	TrajectoryFrame traj;
	trajectory.push_back(traj);

	vector<Point3D> frame;
	//this->do_truc(subset, frame);
	for(int j=0; j<subset->xyz.size(); j++){
		Point3D new_point;
		new_point.raw_pt[0] = subset->xyz[j].x;
		new_point.raw_pt[1] = subset->xyz[j].y;
		new_point.raw_pt[2] = subset->xyz[j].z;
		new_point.pt = new_point.raw_pt;
		new_point.timestamp = subset->ts[j];
		new_point.index_frame = index_frame;

		double r = new_point.raw_pt.norm();
		if ((r > MIN_DIST_LIDAR_CENTER) && (r < MAX_DIST_LIDAR_CENTER)) {
			frame.push_back(new_point);
		}
	}

	//Subsample the scan with voxels taking one random in every voxel
	this->sub_sample_frame(frame, SIZE_VOXEL);

	// The first frame is static
	if (index_frame == 0) {
		trajectory[index_frame].center_R = Eigen::MatrixXd::Identity(3, 3);
		trajectory[index_frame].center_t = Eigen::Vector3d(0., 0., 0.);
	}
	else {
		if (index_frame == 1) {
			trajectory[index_frame].center_R = Eigen::MatrixXd::Identity(3, 3);
			trajectory[index_frame].center_t = Eigen::Vector3d(0., 0., 0.);
		}
		else {
			//Init guess of the next position of the trajectory
			Eigen::Matrix3d R_next_center = trajectory[index_frame - 1].center_R * trajectory[index_frame - 2].center_R.inverse() * trajectory[index_frame - 1].center_R;
			Eigen::Vector3d t_next_center = trajectory[index_frame - 1].center_t + trajectory[index_frame - 1].center_R * trajectory[index_frame - 2].center_R.inverse() * (trajectory[index_frame - 1].center_t - trajectory[index_frame - 2].center_t);
			trajectory[index_frame].center_R = R_next_center;
			trajectory[index_frame].center_t = t_next_center;

			Eigen::Vector3d t_diff = trajectory[index_frame].center_t - trajectory[index_frame-1].center_t;
			if (t_diff.norm() > 5.0) {
				std::cout << "Error in current motion distance !" << std::endl;
			}
		}

		for (int i = 0; i < (int)frame.size(); ++i) {
			frame[i].pt = trajectory[index_frame].center_R * frame[i].raw_pt + trajectory[index_frame].center_t;
		}

		// Use new sub_sample frame as keypoints
		std::list<Point3D> keypoints;
		keypoints.resize(0);
		this->grid_sampling(frame, keypoints, 2.0);
		if (keypoints.size() < NUMBER_KEYPOINTS) {
			keypoints.resize(0);
			this->grid_sampling(frame, keypoints, 1.0);
			if (keypoints.size() < NUMBER_KEYPOINTS) {
				keypoints.resize(0);
				grid_sampling(frame, keypoints, 0.50);
				if (keypoints.size() < NUMBER_KEYPOINTS) {
					keypoints.resize(0);
					grid_sampling(frame, keypoints, 0.20);
				}
			}
		}
		if (keypoints.size() > NUMBER_KEYPOINTS) {
			keypoints.resize(NUMBER_KEYPOINTS);
		}

		//Remove voxels too far from actual position of the vehicule
		vector<Voxel> voxels_to_erase;
		for (std::unordered_map<Voxel, std::list<Eigen::Vector3d>>::iterator it = voxels_map.begin(); it != voxels_map.end(); ++it) {
			Eigen::Vector3d pt = (*it).second.front();
			if ((pt - trajectory[index_frame].center_t).squaredNorm() > (MAX_DIST_MAP * MAX_DIST_MAP)) {
				voxels_to_erase.push_back(it->first);
			}
		}

		//Erase them
		for(int i=0; i<voxels_to_erase.size(); i++){
			voxels_map.erase(voxels_to_erase[i]);
		}

		//Frame To Model
		int number_keypoints_used = frame_to_model(voxels_map, keypoints, trajectory, index_frame);
		if(number_keypoints_used == 0){
			this->reset();
			return;
		}

		//Update frame
		for(int j=0; j<subset->xyz.size(); j++){
			Eigen::Vector3d point;
			point << subset->xyz[j].x, subset->xyz[j].y, subset->xyz[j].z;
			point = trajectory[index_frame].center_R * point + trajectory[index_frame].center_t;
			subset->xyz[j] = vec3(point(0), point(1), point(2));
		}
		subset->pose_T = trajectory[index_frame].center_t;
		subset->pose_R = trajectory[index_frame].center_R;
		subset->root = vec3(subset->pose_T(0), subset->pose_T(1), subset->pose_T(2));
	}

	//Update Voxel Map
	//this->update_voxelMap(frame);
	for (int j=0; j<(int)frame.size(); j++) {
		short kx = static_cast<short>(frame[j].pt[0] / SIZE_VOXEL_MAP);
		short ky = static_cast<short>(frame[j].pt[1] / SIZE_VOXEL_MAP);
		short kz = static_cast<short>(frame[j].pt[2] / SIZE_VOXEL_MAP);

		auto search = voxels_map.find(Voxel(kx, ky, kz));

		if (search != voxels_map.end()) {
			std::list<Eigen::Vector3d>* current_list = &(search->second);

			if ((*current_list).size() < MAX_NUMBER_POINTS_IN_VOXEL) {
				(*current_list).push_back(frame[j].pt);
			}
		}
		else {
			voxels_map[Voxel(kx, ky, kz)].push_back(frame[j].pt);
		}
	}

	this->index_frame++;

	//---------------------------
}
void CT_ICP::reset(){
	//---------------------------

	this->voxels_map.clear();
	this->trajectory.clear();
	this->index_frame = 0;

	//---------------------------
}

// Algo functions
void CT_ICP::do_truc(Subset* subset, vector<Point3D>& frame){
	//---------------------------

	for(int j=0; j<subset->xyz.size(); j++){
		Point3D new_point;
		new_point.raw_pt[0] = subset->xyz[j].x;
		new_point.raw_pt[1] = subset->xyz[j].y;
		new_point.raw_pt[2] = subset->xyz[j].z;
		new_point.pt = new_point.raw_pt;
		new_point.timestamp = subset->ts[j];
		new_point.index_frame = index_frame;

		double r = new_point.raw_pt.norm();
		if ((r > MIN_DIST_LIDAR_CENTER) && (r < MAX_DIST_LIDAR_CENTER)) {
			frame.push_back(new_point);
		}
	}

	//---------------------------
}
void CT_ICP::update_voxelMap(vector<Point3D>& frame){
	//---------------------------

	for (int j=0; j<(int)frame.size(); j++) {
		short kx = static_cast<short>(frame[j].pt[0] / SIZE_VOXEL_MAP);
		short ky = static_cast<short>(frame[j].pt[1] / SIZE_VOXEL_MAP);
		short kz = static_cast<short>(frame[j].pt[2] / SIZE_VOXEL_MAP);

		auto search = voxels_map.find(Voxel(kx, ky, kz));

		if (search != voxels_map.end()) {
			std::list<Eigen::Vector3d>* current_list = &(search->second);

			if ((*current_list).size() < MAX_NUMBER_POINTS_IN_VOXEL) {
				(*current_list).push_back(frame[j].pt);
			}
		}
		else {
			voxels_map[Voxel(kx, ky, kz)].push_back(frame[j].pt);
		}
	}

	//---------------------------
}

//Subfunctions
void CT_ICP::writePoses(string filename, std::vector<TrajectoryFrame> trajectory) {
	ofstream pFile(filename);
	if (pFile.is_open())
	{
		pFile << std::setprecision(std::numeric_limits<long double>::digits10 + 1);
		for (int i = 0; i < (int)trajectory.size(); i++) {
			pFile << trajectory[i].center_t(0) << " " << trajectory[i].center_t(1) << " " << trajectory[i].center_t(2) << std::endl;
		}
		pFile.close();
		return;
	}
	else std::cout << "Unable to open file" << std::endl;
	return;
}
int CT_ICP::frame_to_model(std::unordered_map<Voxel, std::list<Eigen::Vector3d>>& voxels_map, std::list<Point3D>& keypoints, std::vector<TrajectoryFrame>& trajectory, int index_frame) {

	short nb_voxels_visited = 0;
	if (index_frame < 50) {
		nb_voxels_visited = 1;
	}
	else {
		nb_voxels_visited = 1;
	}

	int number_keypoints_used = 0;

	Eigen::Vector3d center_ego = trajectory[index_frame].center_t;

	for (int iter(0); iter < NUMBER_ITER_FRAME_TO_MODEL; iter++) {

		Eigen::MatrixXd A(6, 6);
		Eigen::VectorXd b(6);
		for (int i(0); i < 6; i++) {
			for (int j(0); j < 6; j++) {
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
				int j = 0;
				for (std::list<std::list<Eigen::Vector3d>*>::iterator it_ptr = neighbors_ptr.begin(); it_ptr != neighbors_ptr.end(); ++it_ptr) {
					for (std::list<Eigen::Vector3d>::iterator it = (*it_ptr)->begin(); it != (*it_ptr)->end(); ++it) {
						double sq_dist = (pt_keypoint - (*it)).squaredNorm();
						vector_neighbors.push_back(std::make_pair(sq_dist, (*it)));
						j++;
					}
				}

				std::sort(vector_neighbors.begin(), vector_neighbors.end(), [](const std::pair<double, Eigen::Vector3d>& left, const std::pair<double, Eigen::Vector3d>& right) {return left.first < right.first; });

				int real_number_neighbors = std::min(MAX_NUMBER_NEIGHBORS, (int)vector_neighbors.size());
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

				/*if (normal.dot(trajectory[index_frame].center_t - pt_keypoint) < 0) {
					normal = -1.0 * normal;
				}*/

				//double sigma_1 = sqrt(es.eigenvalues()[2]); //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
				//double sigma_2 = sqrt(es.eigenvalues()[1]);
				//double sigma_3 = sqrt(es.eigenvalues()[0]);
				//double a2D = (sigma_2 - sigma_3) / sigma_1;

				//double lambda_1 = es.eigenvalues()[2]; //Be careful, the eigenvalues are not correct with the iterative way to compute the covariance matrix
				//double lambda_2 = es.eigenvalues()[1];
				//double lambda_3 = es.eigenvalues()[0];
				//double a2D = (lambda_2 - lambda_3) / (lambda_1);

				//double weight_height = 1.0 - (5.0 - min(5.0, (max(-5.0, (*it_keypoint).raw_pt[2])))) / (5.0 + 5.0);
				double weight = 1.0; // weight_height; //a2D;
				Eigen::Vector3d closest_pt_normal = weight * normal;

				Eigen::Vector3d closest_point = vector_neighbors[0].second;

				double dist_to_plane = normal[0] * (pt_keypoint[0] - closest_point[0]) + normal[1] * (pt_keypoint[1] - closest_point[1]) + normal[2] * (pt_keypoint[2] - closest_point[2]);
				//double dist_to_plane = normal[0] * (pt_keypoint[0] - barycenter[0]) + normal[1] * (pt_keypoint[1] - barycenter[1]) + normal[2] * (pt_keypoint[2] - barycenter[2]);

				//Eigen::Vector3d closest_point = pt_keypoint - dist_to_plane * normal;

				double current_max_dist_plane = 0;
				if (index_frame == 1) {
					current_max_dist_plane = current_max_dist_plane = START_DIST_TO_PLANE_FRAME_TO_MODEL - (double)iter / (double)(NUMBER_ITER_FRAME_TO_MODEL - 1) * (START_DIST_TO_PLANE_FRAME_TO_MODEL - MAX_DIST_TO_PLANE_FRAME_TO_MODEL);
				}
				else {
					current_max_dist_plane = MAX_DIST_TO_PLANE_FRAME_TO_MODEL;
				}
				if (fabs(dist_to_plane) < current_max_dist_plane) {

					double scalar = closest_pt_normal[0] * (pt_keypoint[0] - closest_point[0]) + closest_pt_normal[1] * (pt_keypoint[1] - closest_point[1]) + closest_pt_normal[2] * (pt_keypoint[2] - closest_point[2]);
					total_scalar = total_scalar + scalar * scalar;
					mean_scalar = mean_scalar + fabs(scalar);
					number_keypoints_used++;

					Eigen::Vector3d frame_idx_previous_origin = trajectory[index_frame].center_R * (*it_keypoint).raw_pt;

					double cix = (frame_idx_previous_origin[1] * closest_pt_normal[2] - frame_idx_previous_origin[2] * closest_pt_normal[1]);
					double ciy = (frame_idx_previous_origin[2] * closest_pt_normal[0] - frame_idx_previous_origin[0] * closest_pt_normal[2]);
					double ciz = (frame_idx_previous_origin[0] * closest_pt_normal[1] - frame_idx_previous_origin[1] * closest_pt_normal[0]);

					double nix = closest_pt_normal[0];
					double niy = closest_pt_normal[1];
					double niz = closest_pt_normal[2];

					Eigen::VectorXd u(6);
					u << cix, ciy, ciz, nix, niy, niz;
					for (int i = 0; i < 6; i++) {
						for (int j = 0; j < 6; j++) {
							A(i, j) = A(i, j) + u[i] * u[j];
						}
						b(i) = b(i) - u[i] * scalar;
					}
				}
			}
		}
		if (number_keypoints_used < 200) {
			std::cout << "Error : not enough keypoints selected in frame to model !" << std::endl;
			std::cout << "number_keypoints_used : " << number_keypoints_used << std::endl;
			return 0;
		}

		// Normalize equation
		for (int i(0); i < 6; i++) {
			for (int j(0); j < 6; j++) {
				A(i, j) = A(i, j) / number_keypoints_used;
			}
			b(i) = b(i) / number_keypoints_used;
		}

		//Cond number A
		/*Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> es_cond;
		es_cond.compute(A);
		double cond_number_A = es_cond.eigenvalues()[5] / es_cond.eigenvalues()[0];*/

		//Transform Matrix to remove singular elements
		/*for (int i(0); i < 6; i++) {
			A(i, i) = A(i, i);
		}*/


		//Add constraints in trajectory
		if (index_frame > 1)
		{
			Eigen::Vector3d diff_ego = trajectory[index_frame].center_t - center_ego;
			A(3, 3) = A(3, 3) + ALPHA_E;
			A(4, 4) = A(4, 4) + ALPHA_E;
			A(5, 5) = A(5, 5) + ALPHA_E;
			b(3) = b(3) - ALPHA_E * diff_ego(0);
			b(4) = b(4) - ALPHA_E * diff_ego(1);
			b(5) = b(5) - ALPHA_E * diff_ego(2);
		}


		//Solve
		Eigen::VectorXd x = A.ldlt().solve(b);

		double alpha = x(0);
		double beta = x(1);
		double gamma = x(2);
		Eigen::Matrix3d rotation;
		rotation = Eigen::AngleAxisd(alpha, Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(beta, Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(gamma, Eigen::Vector3d::UnitZ());
		Eigen::Vector3d translation = Eigen::Vector3d(x(3), x(4), x(5));

		trajectory[index_frame].center_R = rotation * trajectory[index_frame].center_R;
		trajectory[index_frame].center_t = trajectory[index_frame].center_t + translation;

		//Update keypoints
		for (std::list<Point3D>::iterator it_keypoint = keypoints.begin(); it_keypoint != keypoints.end(); ++it_keypoint) {
			(*it_keypoint).pt = trajectory[index_frame].center_R * (*it_keypoint).raw_pt + trajectory[index_frame].center_t;
		}


		if ((index_frame > 1) && (x.norm() < NORM_X_END_ITERATION_FRAME_TO_MODEL)) {
			return number_keypoints_used;
		}
	}

	return number_keypoints_used;
}
void CT_ICP::sub_sample_frame(std::vector<Point3D>& frame, double size_voxel) {
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
void CT_ICP::grid_sampling(std::vector<Point3D>& frame, std::list<Point3D>& keypoints, double size_voxel_subsampling) {
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
