#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <flann/flann.hpp>


using vector3f_t = Eigen::Vector3f;
using point_t = vector3f_t;
using normal_t = vector3f_t;
using point_cloud_t = std::vector< point_t >;
using normal_cloud_t = std::vector< normal_t >;

using dist_l1_t = flann::L1< float >;
using dist_l2_t = flann::L2< float >;
using index_l1_t = flann::Index< dist_l1_t >;
using index_l2_t = flann::Index< dist_l2_t >;

using corresp_t = std::pair< int, int >;

using matr3f_t = Eigen::Matrix3f;
using matr3d_t = Eigen::Matrix3d;

using rot3f_t = matr3f_t;
using transl3f_t = vector3f_t;
using transf_t = std::pair< rot3f_t, transl3f_t >;
