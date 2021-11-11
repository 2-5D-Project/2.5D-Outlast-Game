#pragma once
#include<iostream>
#include<string>
#define GLFW_STATIC
#include <glad/glad.h>
#include<glfw3.h>
#include<fstream>
#include<sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

class Shader
{
public:
	Shader(const char* vertexPath, const char* fragmentPath);
	string	vertexString;
	string fragmentString;
	const char* vertexSource;
	const char* fragmentSource;
	unsigned int ID;//Shader Program ID
	void setuniform4fv(const char* shaderNameString, glm::mat4 MatName);
	void setuniform1i(const char* shadersampleName, int shadersampleID);
	void setunifor1f(const char* shadersampleName, float shadersampleID);
	void use();
};

