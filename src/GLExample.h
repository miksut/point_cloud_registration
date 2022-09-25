#ifndef Example_GLExample_h
#define Example_GLExample_h

#include "GLApp.h"
#include "Torus.h"
#include "ShaderProgram.h"
#include "MultiLine.h"
#include "Camera.h"
#include "Texture.h"

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "Mesh.h"
#include "FrameBuffer.h"
#include "TextureProcessor.h"
#include "Aligner.h"


namespace cgCourse
{

	class GLExample : public GLApp
	{
	public:

		GLExample(glm::uvec2 _windowSize, std::string _title, std::string _exepath);

		bool init() override;
        bool update(double dt) override;
		bool render() override;
		bool end() override;

	private:
        
        std::shared_ptr<ShaderProgram> programForMeshPhong;

		std::shared_ptr<Torus> torus;
        std::shared_ptr<MultiLine> normalsTorus;

		std::shared_ptr<Texture> torustex;
		std::shared_ptr<Texture> torustexSpec;
        std::shared_ptr<Texture> torusNormaltex;
       
		glm::mat4 mvpMatrix=glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);

		glm::mat4 pre_modelMatrix_core = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
		glm::mat4 pre_modelMatrix = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

		Camera cam;
        
        Mesh armadillo1,armadillo2;

        float pointSize = 5;
		int matrix = -1;

		Aligner modelAligner;
	};
}

#endif
