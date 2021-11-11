#pragma once

#include<glm/glm.hpp>
#include<string>
#include<vector>
#include <glad/glad.h>
#include <unordered_map>

#include "assimp\Importer.hpp"
#include"Shader.h"

typedef unsigned int uint;
#define NUM_BONES_PER_VEREX 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;
	glm::vec3 Normal;
	glm::vec4 boneIds = glm::vec4(0);			//骨骼 
	glm::vec4 boneWeights = glm::vec4(0.0f);	//骨骼的权重
};


struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

// sturction representing an animation track 动画描述
struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};					//移动所费的时间 
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};						//移动
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

struct Bone {
	int id = 0; // position of the bone in final upload array
	std::string name = "";										//名称
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};							//子结点集合
};


// structure containing animation information 动画
struct Animation {
	float duration = 0.0f;														//动画时间
	float ticksPerSecond = 1.0f;												//时间单位
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};	//动画序列map
};

class Mesh
{
public:
	Mesh(float vertices[]);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices);		//带有骨骼动画
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int texture_id = 0;	//默认为0，给一些不带贴图的模型在代码内赋上贴图。例如子弹模型

	void draw(Shader* shader);
	void deleteButter();
	unsigned int VAO;

private:
	unsigned int VBO, EBO,VBO_bone;
	void setupMesh();
};

