#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;

void main()
{
gl_Position=projMat*viewMat* modelMat*vec4(aPos.x,aPos.y,aPos.z,1.0);
vertexColor=vec4(0.4f,0.0f,0.2f,1.0);
TexCoord = aTexCoord;
	
}