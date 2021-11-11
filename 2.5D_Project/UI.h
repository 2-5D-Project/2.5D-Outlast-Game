#pragma once
#include<vector>
#include"Shader.h"
class UI
{
public:
	UI(Shader* _bagshader, Shader* _equipshader);
	Shader* Bagshader;
	Shader* Equipshader;
	unsigned int VAO, VBO;

	vector<string* >Equipments;
	unsigned int equipmentsNumb=0;
	void GetInit();
	void GameStartTextureInit();
	void DrawGameStartTexture(Shader* _shader);
	void Player_Condition(Shader* _shader);
	void GetBagUI();
	void GetGoings();
	void NPCconmmunication();
	void Delete();
	unsigned int GameStartTextureVAO, GameStartTextureVBO;
};

