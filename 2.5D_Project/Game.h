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
	float angle = 0;		//摄像机旋转角度
	bool bo = false;	//检测子弹是否发射
	bool enemybo = false;	//检测敌人子弹是否发射
	int PlayLife = 10;		//玩家血量
	float mouse_gap = 0.0f;	//鼠标点击间隔
	unsigned int Play_injured_interval = 30;	//遇到陷阱时受伤间隔(控制血量下降速度)
	glm::vec3 gameCameraPos;
	glm::vec3 pos;		//游戏当前玩家的位置
	glm::vec3 size;		//模型大小
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
	unsigned int enemy_numpos = 0;	//记录已经赋予位置的敌人数
	//unsigned int enemyCreate = 0;//记录已生成的敌人数
	unsigned int enemyAmount = 3;	//敌人数量
	float enemyYaw = 0;	//敌人朝向
	vector<ObjModel*>EnemyButter;	//敌人子弹
	vector<ObjModel*>Butter;		//玩家子弹
	vector<ObjModel*>Drop;			//掉落物
	vector<float>DropPosx;	//记录敌人死亡前掉落物的位置
	vector<float>DropPosz;	//记录敌人死亡前掉落物的位置
	unsigned int dropAmount = 0;	//掉落物数量
	unsigned int dropCreate = 0;	//掉落物已生成数量
	unsigned int Leve = 1;	//关卡
	unsigned int BoxMatrix = 0;		//箱子矩阵
	GLuint boxamount;		//记录已渲染的箱子数目
	unsigned int amount = 100;	//箱子的数量,决定了着色器中矩阵数组的初始大小,故不能小于箱子矩阵大小
	glm::vec3 wallPos[100];		//墙位置	注意与箱子属性一致
	unsigned int obstaclesMatrix = 0;		//障碍矩阵
	GLuint obstacles_amount;		//记录已渲染的障碍数目
	unsigned int obs_create_amount = 50;	//障碍的数量,决定了着色器中矩阵数组的初始大小,故不能小于箱子矩阵大小
	glm::vec3 obstaclesPos[50];		//障碍位置	注意与障碍属性一致
	Model path;
	

	void PlayInit(Model* _model,Camera _camera);	//玩家绘制
	void GetEnemyModel(Model* _melle_model, Model* _remote_model);		//获取敌人模型
	void EnemyInit();	//敌人
	void EnemyDraw(Shader* enemyshader);		//渲染敌人
	void ButterInit(Camera _camera,Shader* _shader);				//子弹
	void WallInit(ObjModel* _model);	//墙体实例化绑定
	void obstaclesInit(ObjModel* _model);	//障碍实例化绑定
	void WallDraw(Shader* wallshader);	//墙体绘制
	void ObstaclesDraw(Shader* obstaclesshader);	//墙体绘制
	void GetDropModel(ObjModel* _model);	//获取掉落物模型
	void DropInit(Shader* _shader);		//渲染掉落物
	void Play_Enemy_gap();					//玩家与敌人间距离的检测，达到一定距离敌人会有所行动
	void EnemyButterInit(Shader* _shader);		//敌人子弹
	void Melee_weaponsInit(ObjModel* _model,Shader* _shader);	//加载武器
	void NPCInit(ObjModel* _model, Shader* _shader);	//NPC
	void buttercheck();								//子弹碰撞检测
	void melee_weapons_check();				//近战武器的碰撞检测
	bool playercheck();								//玩家碰撞检测(与障碍物)
	void enemyButtercheck();						//敌人子弹的碰撞检测
	bool enemy_Wall();					//检测敌人与墙的碰撞
	void processInput(GLFWwindow* window);
};

