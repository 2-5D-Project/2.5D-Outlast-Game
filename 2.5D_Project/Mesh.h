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
	glm::vec4 boneIds = glm::vec4(0);			//���� 
	glm::vec4 boneWeights = glm::vec4(0.0f);	//������Ȩ��
};


struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

// sturction representing an animation track ��������
struct BoneTransformTrack {
	std::vector<float> positionTimestamps = {};					//�ƶ����ѵ�ʱ�� 
	std::vector<float> rotationTimestamps = {};
	std::vector<float> scaleTimestamps = {};

	std::vector<glm::vec3> positions = {};						//�ƶ�
	std::vector<glm::quat> rotations = {};
	std::vector<glm::vec3> scales = {};
};

struct Bone {
	int id = 0; // position of the bone in final upload array
	std::string name = "";										//����
	glm::mat4 offset = glm::mat4(1.0f);
	std::vector<Bone> children = {};							//�ӽ�㼯��
};


// structure containing animation information ����
struct Animation {
	float duration = 0.0f;														//����ʱ��
	float ticksPerSecond = 1.0f;												//ʱ�䵥λ
	std::unordered_map<std::string, BoneTransformTrack> boneTransforms = {};	//��������map
};

class Mesh
{
public:
	Mesh(float vertices[]);
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	Mesh(std::vector<Vertex> vertices);		//���й�������
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
	unsigned int texture_id = 0;	//Ĭ��Ϊ0����һЩ������ͼ��ģ���ڴ����ڸ�����ͼ�������ӵ�ģ��

	void draw(Shader* shader);
	void deleteButter();
	unsigned int VAO;

private:
	unsigned int VBO, EBO,VBO_bone;
	void setupMesh();
};

