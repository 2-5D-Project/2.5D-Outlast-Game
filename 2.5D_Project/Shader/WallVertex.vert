#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceMatrix;

out vec2 TexCoords;

uniform mat4 viewMat;
uniform mat4 projMat;
void main()
{
    TexCoords = aTexCoords;
    gl_Position = projMat * viewMat * aInstanceMatrix * vec4(aPos, 1.0f); 
}