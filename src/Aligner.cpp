#include "Aligner.h"
#include <iostream>



namespace cgCourse
{
	void Aligner::setPcRef(const std::vector<glm::vec3>& pc) {
		Aligner::pts_pcRef = pc.size();
		for (unsigned int i=0; i < pts_pcRef; i++) {
			point_t vec_pt;
			vec_pt.x() = pc[i].x;
			vec_pt.y() = pc[i].y;
			vec_pt.z() = pc[i].z;
			pcRef.push_back(vec_pt);}
		index_transfMat.push_back(start_matrix);
	};

	void Aligner::setPcAlign(const std::vector<glm::vec3>& pc) {
		Aligner::pts_pcAlign = pc.size();
		for (unsigned int i = 0; i < pts_pcAlign; i++) {
			point_t vec_pt;
			vec_pt.x() = pc[i].x;
			vec_pt.y() = pc[i].y;
			vec_pt.z() = pc[i].z;
			pcAlign.push_back(vec_pt);}
	}

	const std::vector<point_t>& Aligner::getPcRef() {
		return Aligner::pcRef;
	}

	const std::vector<point_t>& Aligner::getPcAlign() {
		return Aligner::pcAlign;
	}

	const std::vector<glm::mat4>& Aligner::getTransfMat() {
		return Aligner::index_transfMat;
	}

	const glm::vec2& Aligner::getError() {
		glm::vec2 error_vec = glm::vec2(error_pre, error);
		return error_vec;
	}

	glm::mat4 Aligner::ICP_iter() {
		// Step 1: Finding correspondences
		knn_queries knnIndex(pcRef); // Creating a kd-tree as index structure

		for (unsigned int i = 0; i < pts_pcAlign; i++) { // find nn for every point in pcAlign
			int index = knnIndex.query_nn(pcAlign[i]);
			nn_indices.push_back(index);}
		
		// Step 2: Computation of rigid transform given the correspondences
		mu_ref << 0.0, 0.0, 0.0;
		for (unsigned int i = 0; i < pts_pcAlign; i++) { // Only considering the points in pc_Ref that belong to the correspondences
			mu_ref += pcRef[nn_indices[i]];
		}
		mu_ref /= pts_pcAlign;
		
		mu_align << 0.0, 0.0, 0.0;
		for (unsigned int i = 0; i < pts_pcAlign; i++) {
			mu_align += pcAlign[i];}
		mu_align /= pts_pcAlign;

		C << 0, 0, 0, 0, 0, 0, 0, 0, 0;
		for (unsigned int i = 0; i < pts_pcAlign; i++) {
			C += (pcAlign[i] - mu_align)*((pcRef[nn_indices[i]] - mu_ref).transpose());}

		Eigen::JacobiSVD<matr3f_t> svd(C, Eigen::DecompositionOptions::ComputeFullU | Eigen::DecompositionOptions::ComputeFullV);
		V << svd.matrixV().transpose();
		U_transpose << svd.matrixU().transpose();
		sigma_adjusted << 1, 0, 0,  0, 1, 0,  0, 0, (V*U_transpose).determinant();
		rotation << V * sigma_adjusted * U_transpose;
		translation << mu_ref - (rotation * mu_align);
		
		for (unsigned int i = 0; i < 3; i++) {
			for (unsigned int j = 0; j < 3; j++) {
				transformation[j][i] = rotation(i, j);}}

		for (unsigned int i = 0; i < 3; i++) {
			transformation[3][i] = translation(i);}

		transformation = transformation * transformation_pre;
		index_transfMat.push_back(transformation);
		transformation_pre = transformation;

		// Step 3: Error calculation and adjustment of points for next iteration
		error_pre = error;
		
		for (unsigned int i = 0; i < pts_pcAlign; i++) {
			pcAlign[i] = rotation * pcAlign[i] + translation;}

		float norm_squared = 0;
		for (unsigned int i = 0; i < pts_pcAlign; i++) {
			norm_squared += (pcAlign[i] - pcRef[nn_indices[i]]).squaredNorm();}
		error = norm_squared;
		iteration += 1;

		nn_indices.clear();

		std::cout << "The error is: " << error << std::endl;
		std::cout << "Iteration: " << iteration << std::endl;
		std::cout << "\n" << std::endl;

		return index_transfMat.back();
	}

}
