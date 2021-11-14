#pragma once
#include<math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include"Model.h"
#include"Shader.h"
#include"ObjModel.h"
#include"CWindow.h"

struct All_Enemy
{
	Model* enemy;
	string type;
};

class Game
{
public:
	Game(Shader* _shader, Camera _camera,CWindow _wnd ,glm::vec3 _pos,glm::vec3 _size);
	CWindow Wnd;
	TCHAR Buffer[128];
	float angle = 0;		//�������ת�Ƕ�
	bool bo = false;	//����ӵ��Ƿ���
	bool enemybo = false;	//�������ӵ��Ƿ���
	int PlayLife = 10;		//���Ѫ��
	float mouse_gap = 0.0f;	//��������
	unsigned int Play_injured_interval = 30;	//��������ʱ���˼��(����Ѫ���½��ٶ�)
	glm::vec3 gameCameraPos;
	glm::vec3 pos;		//��Ϸ��ǰ��ҵ�λ��
	glm::vec3 size;		//ģ�ʹ�С
	glm::vec3 Playersize;
	float Buttersize;
	float Wallsize;
	Camera camera;
	Camera Gamecamera;
	Shader* Playershader;	
	Shader* Buttershader;
	Model* Player;
	All_Enemy Melleenemy, Remote_enemy;
	ObjModel* Melee_weapons;
	ObjModel* Wall;
	ObjModel* Obstacles;
	ObjModel* NPC;
	vector<All_Enemy>ALL_enemy;
	//vector<Model*> Enemy;
	Model* Enemy_;
	unsigned int enemy_numpos = 0;	//��¼�Ѿ�����λ�õĵ�����
	//unsigned int enemyCreate = 0;//��¼�����ɵĵ�����
	unsigned int enemyAmount = 3;	//��������
	float enemyYaw = 0;	//���˳���
	vector<ObjModel*>EnemyButter;	//�����ӵ�
	vector<ObjModel*>Butter;		//����ӵ�
	vector<ObjModel*>Drop;			//������
	vector<float>DropPosx;	//��¼��������ǰ�������λ��
	vector<float>DropPosz;	//��¼��������ǰ�������λ��
	unsigned int dropAmount = 0;	//����������
	unsigned int dropCreate = 0;	//����������������
	unsigned int Leve = 1;	//�ؿ�
	unsigned int BoxMatrix = 0;		//���Ӿ���
	GLuint boxamount;		//��¼����Ⱦ��������Ŀ
	unsigned int amount = 100;	//���ӵ�����,��������ɫ���о�������ĳ�ʼ��С,�ʲ���С�����Ӿ����С
	glm::vec3 wallPos[100];		//ǽλ��	ע������������һ��
	unsigned int obstaclesMatrix = 0;		//�ϰ�����
	GLuint obstacles_amount;		//��¼����Ⱦ���ϰ���Ŀ
	unsigned int obs_create_amount = 50;	//�ϰ�������,��������ɫ���о�������ĳ�ʼ��С,�ʲ���С�����Ӿ����С
	glm::vec3 obstaclesPos[50];		//�ϰ�λ��	ע�����ϰ�����һ��
	Model path;
	

	void PlayInit(Model* _model,Camera _camera);	//��һ���
	void GetEnemyModel(Model* _melle_model, Model* _remote_model);		//��ȡ����ģ��
	void EnemyInit();	//����
	void EnemyDraw(Shader* enemyshader);		//��Ⱦ����
	void ButterInit(Camera _camera,Shader* _shader);				//�ӵ�
	void WallInit(ObjModel* _model);	//ǽ��ʵ������
	void obstaclesInit(ObjModel* _model);	//�ϰ�ʵ������
	void WallDraw(Shader* wallshader);	//ǽ�����
	void ObstaclesDraw(Shader* obstaclesshader);	//ǽ�����
	void GetDropModel(ObjModel* _model);	//��ȡ������ģ��
	void DropInit(Shader* _shader);		//��Ⱦ������
	void Play_Enemy_gap();					//�������˼����ļ�⣬�ﵽһ��������˻������ж�
	void EnemyButterInit(Shader* _shader);		//�����ӵ�
	void Melee_weaponsInit(ObjModel* _model,Shader* _shader);	//��������
	void NPCInit(ObjModel* _model, Shader* _shader);	//NPC
	void buttercheck();								//�ӵ���ײ���
	void melee_weapons_check();				//��ս��������ײ���
	bool playercheck();								//�����ײ���(���ϰ���)
	void enemyButtercheck();						//�����ӵ�����ײ���
	bool enemy_Wall();					//��������ǽ����ײ
	void processInput(GLFWwindow* window);
};

