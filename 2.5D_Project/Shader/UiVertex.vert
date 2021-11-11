#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;;

out vec4 vertexColor;
out vec2 TexCoord;

#define UI_MODEL 2

//struct ModelMat
//{
//	mat4 modelMat;
//};
//uniform ModelMat model[UI_MODEL];

uniform mat4 modelMat;
uniform mat4 viewMat;
uniform mat4 projMat;


void main()
{
	mat4 Y=mat4(1.0f);
	gl_Position=modelMat*vec4(aPos.x,aPos.y,aPos.z,1.0);
	TexCoord = aTexCoord;
	
}