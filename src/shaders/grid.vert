#version 330 core

layout (location = 0) in vec3 position;

uniform mat4 u_Model = mat4(1.0f);

layout (std140) uniform ViewProjectionData
{
    mat4 u_View;
    mat4 u_Projection;
};

void main()
{
    gl_Position = u_Projection * u_View * u_Model * vec4(position, 1.0f);
}