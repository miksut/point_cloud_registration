#ifndef Aligner_h
#define Aligner_h

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "Registration/knn_queries.hpp"
#include <Eigen/Core>
#include <Eigen/Dense>


namespace cgCourse
{
	class Aligner {
	public:
		void setPcRef(const std::vector<glm::vec3>& pc);
		void setPcAlign(const std::vector<glm::vec3>& pc);
		const std::vector<point_t>& getPcRef();
		const std::vector<point_t>& getPcAlign();
		const glm::vec2& getError();
		const std::vector<glm::mat4>& getTransfMat();
		glm::mat4 ICP_iter();

		float iteration = 0;

	private:
		std::vector <point_t> pcRef;
		std::vector <point_t> pcAlign;
		unsigned int pts_pcRef; // number of pts in reference PC
		unsigned int pts_pcAlign;

		std::vector <int> nn_indices; // indices of nearest neighbors in reference PC

		point_t mu_ref;
		point_t mu_align;
		matr3f_t C;
		matr3f_t sigma_adjusted;
		matr3f_t V;
		matr3f_t U_transpose;
		rot3f_t rotation; // rotation matrix
		transl3f_t translation; // translation vector

		glm::mat4 transformation_pre;
		glm::mat4 transformation; // composed transformation matrix
		std::vector<glm::mat4> index_transfMat;
		glm::mat4 start_matrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		float error = 10000000; // initialization hardcoded as starting point, may in extreme cases need to be adjusted
		float error_pre = 0;

	};
}

#endif
