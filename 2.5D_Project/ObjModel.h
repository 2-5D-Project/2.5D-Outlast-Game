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
	float Butter_yaw;		//记录子弹转向
	float enemyButter_yaw_x;		//记录敌人子弹转向	(子弹与玩家之间需要发生的偏移)
	float enemyButter_yaw_z;		//记录子弹转向
	bool  Butter_Fight = false;	//记录已经子弹是否已被赋予朝向值 （子弹是始终朝着人物前方发射）	
	Shader* shader;
	Camera camera;
	glm::vec3 butterpos;	//子弹位置
	glm::vec3 butterYawPos;		//计算子弹转向		
	glm::vec3 NPCpos;
	float butterAttack = 5.0f;		//子弹攻击力
	float melee_weapons_trans=0;		//近战武器下挥频率
	glm::vec3 TransBox;		//传送门位置
	
	glm::vec3 dropPos=glm::vec3(0,0,0);
	vector<Texture> textures_loaded;
	std::string directoryTexture;		//obj模型路径

	void butterdraw(Shader* shader, float Yaw);			//子弹
	void enemybutterdraw(Shader* shader, glm::vec3 playpos); //敌人子弹
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

