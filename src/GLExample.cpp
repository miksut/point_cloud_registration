#include "GLExample.h"
#include <iostream>
#include <fstream>


#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include "Gui.h"
#include "Registration/common.h"
#include "Aligner.h"




namespace cgCourse
{
    Assimp::Importer importer;
    
    GLExample::GLExample(glm::uvec2 _windowSize, std::string _title, std::string _exepath)
    : GLApp(_windowSize, _title, _exepath, false)
    {
        this->mvpMatrix = glm::mat4(1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1);
    }
    
    
    bool GLExample::init()
    {

        initGui(window_);
        
        connectVar("pointSize",&pointSize);
		connectVar("matrix", &matrix);
        
        // Framebuffer size and window size may be different in high-DPI displays
        // setup camera with standard view (static for our case)
        this->cam.create(this->getFramebufferSize(),this->getWindowSize(),
                         glm::vec3(0, 0, -3),
                         glm::vec3(0, 0, 0),
                         glm::vec3(0, 1, 0));
        InputManager::registerInputAcceptor(&cam);
        connectVar("cameraType", &cam.getType());
        
        this->torus = std::make_shared<Torus>();
        this->torus->createVertexArray(0, 1, 2, 3, 4);
        
        this->torus->setPosition(glm::vec3(-1, 0.0, 0.0));
        this->torus->setScaling(glm::vec3(0.1, 0.1, 0.1));
        this->torus->getMaterial()->ka = glm::vec3(1,0,0);
        
        if(this->torus->getNormals().size() > 0) {
            this->normalsTorus = std::make_shared<MultiLine>(this->torus->getPositions(), this->torus->getNormals(), torus->getTangents());
            this->normalsTorus->createVertexArray(0, 1, 2, 3, 4);
            this->normalsTorus->setScaling(glm::vec3(0.1, 0.1, 0.1));
            this->normalsTorus->setPosition(glm::vec3(-1, 0.0, 0.0));
            normalsTorus->setMaterial(std::make_shared<Material>());
            normalsTorus->getMaterial()->ka=glm::vec3(1,1,1);
        }
        
        programForMeshPhong = std::make_shared<ShaderProgram>(this->getPathToExecutable() + "../../shader/Mesh_phong");
        
        std::string unaligned_filename("armadillo_unaligned.ply");   // the cloud that will move
        std::string ref_filename("armadillo_ref.ply");     // the reference cloud
        
        
        //TODO: initialize model aligner
        //Tip: Create a separate class for the model aligner

        armadillo1.load(this->getPathToExecutable() + "../../res/RegistrationData/",ref_filename,true,false,true);
        armadillo1.setPosition(glm::vec3(0,0,0));
        armadillo1.setScaling(glm::vec3(.01,.01,.01));
        armadillo1.setMaterial(std::make_shared<Material>());
        armadillo1.getMaterial()->ka=glm::vec3(0,1,0.5);
        
        armadillo2.load(this->getPathToExecutable() + "../../res/RegistrationData/",unaligned_filename,true,false,true);
        armadillo2.setPosition(glm::vec3(0,0,0));
        armadillo2.setScaling(glm::vec3(.01,.01,.01));
        armadillo2.setMaterial(std::make_shared<Material>());
        armadillo2.getMaterial()->ka=glm::vec3(1,1,0);

		// Initializing model aligner (see separate class -> files Aligner.h and Aligner.cpp)
		this->modelAligner.setPcRef(armadillo1.elements[0]->getPositions());
		this->modelAligner.setPcAlign(armadillo2.elements[0]->getPositions());

        this->torusNormaltex = std::make_shared<Texture>();
        this->torusNormaltex->loadFromFile(this->getPathToExecutable() + "../../res/brickwall_normal.jpg");
        this->torus->getMaterial()->normalTexture =torusNormaltex;
        
        this->torustex = std::make_shared<Texture>();
        this->torustex->loadFromFile(this->getPathToExecutable() + "../../res/brickwall.jpg");
        this->torus->getMaterial()->diffuseTexture = torustex;
        this->torustexSpec = std::make_shared<Texture>();
        this->torustexSpec->loadFromFile(this->getPathToExecutable() + "../../res/brickwall_specular.jpg");
        this->torus->getMaterial()->specTexture = torustexSpec;
        
        std::cout<<"Frame Buffer Size:"<<getFramebufferSize().x<<", "<<getFramebufferSize().y<<std::endl;
         
        return true;
    }
    
    bool GLExample::update(double dt)
    {
        //TODO: run the next iteration of the model aligner
		if ((modelAligner.getError().x - modelAligner.getError().y) > 0 || (modelAligner.getError().x - modelAligner.getError().y) == -(modelAligner.getError().y)) {
			pre_modelMatrix = modelAligner.ICP_iter();
		}
		else { pre_modelMatrix = modelAligner.getTransfMat()[modelAligner.iteration - 1]; }

        updateGui();
        
        glPointSize(pointSize);

        return true;
    }
    
    bool GLExample::render()
    {
        glDisable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_DEPTH_TEST);
        
        // Clear the color and depth buffers
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        bindDefaultFrameBuffer(getFramebufferSize());
        clearDefaultFrameBuffer();
        
        //TODO: render the full model and the aligned model based on the GUI slider or the iteration
        armadillo1.draw(this->cam.getProjectionMatrix(),this->cam.getViewMatrix(),programForMeshPhong);
		if (matrix > -1) {
			this->programForMeshPhong->setUniformMat4fv("pre_modelMatrix", modelAligner.getTransfMat()[abs(matrix-9)]);}
		else { this->programForMeshPhong->setUniformMat4fv("pre_modelMatrix", this->pre_modelMatrix); }
        armadillo2.draw(this->cam.getProjectionMatrix(),this->cam.getViewMatrix(),programForMeshPhong);
		this->programForMeshPhong->setUniformMat4fv("pre_modelMatrix", this->pre_modelMatrix_core);
        this->torus->draw(this->cam.getProjectionMatrix(),this->cam.getViewMatrix(),programForMeshPhong);
        this->normalsTorus->draw(this->cam.getProjectionMatrix(),this->cam.getViewMatrix(),programForMeshPhong);
        renderGui();
        return true;
    }
    
    bool GLExample::end()
    {
        //    programForCube->deleteShaderProgramFromGPU();
        //    programForTorus->deleteShaderProgramFromGPU();
        //    programForTorusNormals->deleteShaderProgramFromGPU();
        return true;
    }
}

