#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	ifstream vertexFile;
	ifstream fragmentFile;
	stringstream vertexSStream;
	stringstream fragmentSStream;

	vertexFile.open(vertexPath);
	fragmentFile.open(fragmentPath);
	try
	{
		if (!vertexFile.is_open() || !fragmentFile.is_open()) {
			throw exception("open file error");
		}
		vertexSStream << vertexFile.rdbuf();
		fragmentSStream << fragmentFile.rdbuf();

		vertexString = vertexSStream.str();
		fragmentString = fragmentSStream.str();

		vertexSource = vertexString.c_str();
		fragmentSource = fragmentString.c_str();

		unsigned int vertex, fragment;
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertexSource, NULL);
		glCompileShader(vertex);

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragmentSource, NULL);
		glCompileShader(fragment);

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);

	}
	catch (const std::exception& ex)
	{
		printf(ex.what());
	}

}


void Shader::setuniform4fv(const char* shaderNameString, glm::mat4 MatName)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, shaderNameString), 1, GL_FALSE, glm::value_ptr(MatName));
}

void Shader::setuniform1i(const char* shadersampleName, int shadersampleID)
{
	glUniform1i(glGetUniformLocation(ID, shadersampleName), shadersampleID);
}

void Shader::setunifor1f(const char* shadersampleName, float shadersampleID)
{
	glUniform1f(glGetUniformLocation(ID, shadersampleName), shadersampleID);
}

void Shader::use() {

	glUseProgram(ID);
}