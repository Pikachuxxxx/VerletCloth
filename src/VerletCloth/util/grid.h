#pragma once

#include <vector>

#include "Shader.h"

class Grid
{
public:  
    int                     slices;
private:
    std::vector<glm::vec3>  m_Vertices;
    std::vector<glm::uvec4> m_Indices;

    GLuint                  m_VAO;
    GLuint                  m_VBO;
    GLuint                  m_IBO;

    Shader*                 m_DefaultShader;
public:
    Grid(int slices = 10);

    void render();
private:
    inline GLuint getIndicesCount() const { return (GLuint)m_Indices.size() * 4; }
};

