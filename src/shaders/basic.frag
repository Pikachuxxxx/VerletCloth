#version 410 core

in vec2 uvCoords;

out vec4 color;

uniform sampler2D clothTex;

void main()
{
    color = vec4(0.8, 0.1, 0.1, 1);
    color = texture(clothTex, uvCoords);
}
