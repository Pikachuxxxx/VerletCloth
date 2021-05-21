#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "Camera3D.h"
#include "Renderer.h"

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// ImGui
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Editor
{
public:
    Camera3D* sceneCamera;
    double* deltaTime;
    bool enableWireFrameMode;
    glm::vec3* wind;
private:
    Transform* m_Transform;
    glm::vec3 p;
public:
    Editor(GLFWwindow* window);
    ~Editor();

    void render();

    void SetGlobalStats(Camera3D* camera3d, double * deltaTime);
    void SetTransformVariable(Transform* transform);
private:
    void DrawGlobalStats();
    void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 150.0f);
    void DrawVec3RGBControl(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 150.0f);
    void DrawVec4RGBControl(const std::string& label, glm::quat& values, float resetValue = 0.0f, float columnWidth = 150.0f);
};

