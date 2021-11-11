#pragma once
#include <vector>
#include"Shader.h"

class Floor
{public:

	Floor(Shader* _shader);
	Shader* shader;
	int row_num = 200, col_num = 200;
	//vector<float> floorVertices;
	//vector<unsigned int> floorElement;
	
	void FloorVertices();
	void Init();
	void NoiseInit();
	void TransDoorInit();
	void draw();
	void drawNoise();
	void drawTransDoor();
	void drawDianJi();
	unsigned int TextureID = 10;	//载入贴图的序号
	int TransTime = 100;//传送所需时间
	unsigned int DoorTextureID = 10;		//传送门贴图起始ID
	unsigned int DianjiTextureID = 134;			//电击贴图
	float TextureGap = 2.0f;		//贴图切换的时间间隔
	float DoorTextureGap = 2.0f;		//贴图切换的时间间隔
	float DianJiTextureGap = 2.0f;		//闪电贴图切换的时间间隔
	glm::vec3 NoisePos = glm::vec3(9.0,0.01f,12.0f);
	glm::vec3 TransDoorPos = glm::vec3(6.0, 0.01f, 4.0f);
	glm::vec3 DianJiTransPos = glm::vec3(6.0, 0.01f, 4.0f);
	unsigned int floorVAO, floorVBO, floorEBO;
	unsigned int noiseVAO, noiseVBO, noiseEBO;
	unsigned int TransDoorVAO, TransDoorVBO;
};

