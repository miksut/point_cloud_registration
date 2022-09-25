#include "MultiLine.h"
#include <iostream>

// to get M_PI from math libary enable defines
#define _USE_MATH_DEFINES
#include <math.h>
#include "ShaderProgram.h"

namespace cgCourse
{
	MultiLine::MultiLine(const std::vector< glm::vec3 >& _vertices,
                         const std::vector< glm::vec3 >& _normals,
                         const std::vector< glm::vec3 >& _tangents) : DrawableShape()
	{
        // calculate positions for vertex normals
		for (int j = 0; j < _vertices.size(); j++)
		{
            // adding a normal per vertex
            this->positions.push_back(_vertices[j]);
            this->positions.push_back(_vertices[j] + (_normals[j] * 0.2f));
            // line indices add for every line 2 indices from tail to head
            lineIndices.push_back(glm::uvec2((j*4),(j*4)+1));
            this->colors.push_back(glm::vec3(1.0,1.0,0.0));
            this->colors.push_back(glm::vec3(1.0,1.0,0.0));
            
            // adding a tangents per vertex
            this->positions.push_back(_vertices[j]);
            this->positions.push_back(_vertices[j] + (_tangents[j] * 0.2f));
            // line indices add for every line 2 indices from tail to head
            lineIndices.push_back(glm::uvec2((j*4)+2,(j*4)+3));
            this->colors.push_back(glm::vec3(1.0,0.0,1.0));
            this->colors.push_back(glm::vec3(1.0,0.0,1.0));
            primitiveType = line;
            
		}
	}

}
