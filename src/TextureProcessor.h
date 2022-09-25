//
//  TextureProcessor.hpp
//  CGL
//
//  Created by Alireza Amiraghdam on 11.06.19.
//

#ifndef TextureProcessor_hpp
#define TextureProcessor_hpp

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>
#include "GLIncludes.h"
#include "ShaderProgram.h"


namespace cgCourse
{
    namespace TextureProcess{
    struct VertexData{
        float x,y,z;   //vertex coordinates
        float tcoorx,tcoory;
    };
        void init();
        void run(std::vector<unsigned int> _textures,std::shared_ptr<ShaderProgram> processProgram);
    
    }
    
}
#endif /* TextureProcessor_hpp */
