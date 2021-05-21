#include "grid.h"

Grid::Grid(int slices /*= 10*/)
{
    for (int j = -slices / 2; j <= slices / 2; ++j) {
        for (int i = -slices / 2; i <= slices / 2; ++i) {
            float x = (float)i / (float)slices * (slices /10.0f);
            float y = 0;
            float z = (float)j / (float)slices * (slices / 10.0f);
            m_Vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for (int j = 0; j < slices; ++j) {
        for (int i = 0; i < slices; ++i) {

            int row1 = j * (slices + 1);
            int row2 = (j + 1) * (slices + 1);

            m_Indices.push_back(glm::uvec4(row1 + i, row1 + i + 1, row1 + i + 1, row2 + i + 1));
            m_Indices.push_back(glm::uvec4(row2 + i + 1, row2 + i, row2 + i, row1 + i));

        }
    }

    m_DefaultShader = new Shader("./src/shaders/grid.vert", "./src/shaders/grid.frag");


    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3), glm::value_ptr(m_Vertices[0]), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &m_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(glm::uvec4), glm::value_ptr(m_Indices[0]), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void Grid::render()
{
    m_DefaultShader->Use();

    glBindVertexArray(m_VAO);
    glDrawElements(GL_LINES, getIndicesCount(), GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}
