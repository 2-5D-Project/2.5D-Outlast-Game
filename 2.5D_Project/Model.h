#pragma once
#include"Mesh.h"
#include<vector>
#include<iostream>
#include<string>
#include<map>
#include "glm\gtc\quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include"Shader.h"
#include"Camera.h"

class Model
{
public:
	static const uint MAX_BONES = 100;
	float Jtime = 0.6f;
	glm::vec3 pos;	//play��λ��
	float enemy_yaw_x;	//��¼����ת��
	float enemy_yaw_z;
	float enemyRotAngle =0.0f;
	Shader* shader;
	Camera camera;
	Model* enemy;
	unsigned int nBoneCount = 0;
	Bone skeleton;		//�Ǽ�
	glm::mat4 globalInverseTransform;
	glm::mat4 identity=glm::mat4(1.0f);
	std::vector<glm::mat4> currentPose = {};
	Animation animation;
	std::vector<Mesh> meshes;
	vector<Texture> textures_loaded;
	glm::vec3 enemyPos;			//����λ��
	//vector<ObjModel> enemyButter;	//�����ӵ�
	
	bool enemy_shootButter = false;		//�������Ƿ����ӵ�
	float enemybutterRate = 0;	//�ӵ�����Ƶ��
	//unsigned int enemyAmount = 3;	//��������
	float enemyBlood = 5.0f;	//����Ѫ��
	unsigned int enemyLive = 1;	//�жϵ����Ƿ��� 1Ϊ�0Ϊ��

	Model();
	Model(std::string path);
	Model(std::string path,glm::vec3 _Modelpos);
	std::string directory;
	std::string directoryTexture;		//objģ��·��
	void draw(Shader* shader, glm::vec3 _pos,Camera _camera);	//��Ⱦ���
	void enemydraw(Shader* shader,glm::vec3 playpos);					//����
	//void enemyButterdraw();		//�����ӵ�
	void Action(float elapsedTime);
private:

	
	inline glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat);
	inline glm::vec3 assimpToGlmVec3(aiVector3D vec);
	inline glm::quat assimpToGlmQuat(aiQuaternion quat) {
		glm::quat q;
		q.x = quat.x;
		q.y = quat.y;
		q.z = quat.z;
		q.w = quat.w;

		return q;
	};

	void loadMode(std::string path);

	bool readSkeleton(Bone& boneOutput, aiNode* node, std::unordered_map<std::string, std::pair<int, glm::mat4>>& boneInfoTable);
	void loadAnimation(const aiScene* scene, Animation& animation);
	std::pair<uint, float> getTimeFraction(std::vector<float>& times, float& dt);
	void getPose(Animation& animation, Bone& skeletion, float dt, std::vector<glm::mat4>& output, glm::mat4& parentTransform, glm::mat4& globalInverseTransform);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName, const  aiScene* scene);
	unsigned int TextureFromFile(const char* path, const string& directory, bool gamma= false);

};
