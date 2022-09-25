//
//  TextureProcessor.cpp
//  CGL
//
//  Created by Alireza Amiraghdam on 11.06.19.
//

#include "TextureProcessor.h"
namespace cgCourse
{
    namespace TextureProcess{
    std::vector< VertexData > vertices;
    std::vector<unsigned int> indices;
    unsigned int gl_vertex_array_object, gl_vertex_buffer_object, gl_index_buffer_object;

    void init(){
        vertices = {
            {-1.0f, -1.0f, 0.0f, 0.0f, 0.0f},
            {1.0f, -1.0f, 0.0f, 1.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 0.0f, 1.0f, 1.0f}
        };
        indices = {0,1,2,3};

        //creat OpenGL objects to use in drawing
        
        
        //vertex array object
        glGenVertexArrays(1, &gl_vertex_array_object);
        glBindVertexArray(gl_vertex_array_object);
        
        //vertex buffer object -> we hold the vertices
        glGenBuffers(1,&gl_vertex_buffer_object);
        glBindBuffer(GL_ARRAY_BUFFER, gl_vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);
        
        
        //index buffer object -> we hold the index of vertex
        glGenBuffers(1,&gl_index_buffer_object);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gl_index_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
        
        //the connection between attributes, interleaved data
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)0);                       //send positions on pipe 0
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(VertexData),(void*)(sizeof(float)*3));       //send normals on pipe 1
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    
    void run(std::vector<unsigned int> _textures,std::shared_ptr<ShaderProgram> processProgram){
        glBindVertexArray(gl_vertex_array_object);
        for (int i =0 ;i<_textures.size();i++){
            processProgram->addTexture("input"+std::to_string(i), _textures[i]);
        }
        processProgram->bind();
        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        
    }
    }
}
