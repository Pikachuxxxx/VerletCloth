#version 410 core

layout (location = 0) in vec3 position;
layout (location = 1 )in vec2 uv;

uniform mat4 u_Model = mat4(1.0f);

layout (std140) uniform ViewProjectionData
{
    mat4 u_View;
    mat4 u_Projection;
};

out vec2 uvCoords;

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0f);
    uvCoords = uv;
}
