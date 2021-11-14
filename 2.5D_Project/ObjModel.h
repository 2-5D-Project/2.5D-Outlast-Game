#pragma once
#include"Mesh.h"
#include<vector>
#include<iostream>
#include<string>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include <fstream>
#include <sstream>

#include"Shader.h"
#include"Camera.h"
class ObjModel
{
public:
	std::vector<Mesh> meshes;
	ObjModel();
	ObjModel(std::string path);
	ObjModel(std::string path, glm::vec3 _Modelpos);
	float Butter_yaw;		//��¼�ӵ�ת��
	float enemyButter_yaw_x;		//��¼�����ӵ�ת��	(�ӵ������֮����Ҫ������ƫ��)
	float enemyButter_yaw_z;		//��¼�ӵ�ת��
	bool  Butter_Fight = false;	//��¼�Ѿ��ӵ��Ƿ��ѱ����賯��ֵ ���ӵ���ʼ�ճ�������ǰ�����䣩	
	Shader* shader;
	Camera camera;
	glm::vec3 butterpos;	//�ӵ�λ��
	glm::vec3 butterYawPos;		//�����ӵ�ת��		
	glm::vec3 NPCpos;
	float butterAttack = 5.0f;		//�ӵ�������
	float melee_weapons_trans=0;		//��ս�����»�Ƶ��
	glm::vec3 TransBox;		//������λ��
	
	glm::vec3 dropPos=glm::vec3(0,0,0);
	vector<Texture> textures_loaded;
	std::string directoryTexture;		//objģ��·��

	void butterdraw(Shader* shader, float Yaw);			//�ӵ�
	void enemybutterdraw(Shader* shader, glm::vec3 playpos); //�����ӵ�
	void wallInit();
	void obstaclesInit();
	void dropdraw(Shader* shader);
	void NPCdraw(Shader* shader);
	void melee_weapons_draw(Shader* shader, glm::vec3 weaponsInitPos, float yaw);
private:
	void loadMode(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const  aiScene* scene);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false);
};

