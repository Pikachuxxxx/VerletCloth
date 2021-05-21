#include <iostream>

#include <Renderer.h>
#include <Window.h>
#include "util/grid.h"
#include "../editor/editor.h"

#include "Cloth.h"

int main()
{
    Window window("Cloth Simulation Engine", 1200, 800);

    Camera3D camera(glm::vec3(-1.0f, 2.62f, 2.0f), glm::vec3(0.2f, 0.8f, -0.5f), -60.0f, -34.0f);

    glm::mat4 View = glm::mat4(1.0f);
    glm::mat4 Projection = glm::perspective(45.0f, (float)window.getWidth() / (float)window.getHeight(), 0.1f, 100.0f);
    Renderer renderer(View, Projection);

    Shader* meshShader = new Shader("./src/shaders/basic.vert", "./src/shaders/basic.frag");
    Texture2D* clothTex = new Texture2D("./res/checker_map.png");

    Transform ClothTransform(glm::vec3(0, 1.5f, 0), glm::vec3(0), glm::vec3(0.1f));
    glm::vec3 Wind = glm::vec3(0, -0.1f, 0.2f);

    Editor editor(window.getGLFWwindow());
    editor.SetGlobalStats(&camera, &window.deltaTime);
    editor.SetTransformVariable(&ClothTransform);
    editor.wind = &Wind;

    Grid grid(50);

    Cloth cloth(15, 15, 15, 15);
    cloth.CalculateClothVertices();

    VertexArray cloth_VAO;
    VertexBuffer cloth_VBO(NULL, cloth.vertices.size() * sizeof(float) * 3 + cloth.uv.size() * sizeof(float) * 2);
    VertexBufferLayout clothLayout;
    clothLayout.Push<float>(3);
    clothLayout.Push<float>(2);
    cloth_VAO.AddBuffer(cloth_VBO, clothLayout);
    cloth_VAO.Unbind();
    cloth_VBO.Unbind();

    struct VertexData
    {
        glm::vec3 position;
        glm::vec2 uv;
    };

    glPointSize(10.0f);

    while (!window.closed())
    {
        window.backgroundColor = glm::vec4(0.21, 0.21, 0.21, 1.0f);
        glEnable(GL_DEPTH_TEST);
        window.clear();
        camera.Update(window);
        View = camera.GetViewMatrix();
        renderer.set_VP_uniform_buffers(View, Projection);

        grid.render();

        cloth.AddForce(glm::vec3(0, -5.0f, 0));
        cloth.AddWindForce(Wind);
        cloth.TimeStep();
        cloth.CalculateClothVertices();

        cloth_VBO.Bind();
        VertexData* buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        if (buffer == nullptr)
        {
            std::cerr << "ERROR::OPENGL:: Cannot Map Vertex Buffer Memory" << std::endl;
        }
        for (unsigned int i = 0; i < cloth.vertices.size(); i++)
        {
            buffer->position = cloth.vertices[i];
            buffer->uv = cloth.uv[i];
            buffer++;
        }
        glUnmapBuffer(GL_ARRAY_BUFFER);
        cloth_VBO.Unbind();

        if (editor.enableWireFrameMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        renderer.draw_raw_arrays_with_textures(ClothTransform, *meshShader, *clothTex, cloth_VAO, cloth.vertices.size());

        editor.render();

        window.update();
    }

    return EXIT_SUCCESS;
}
