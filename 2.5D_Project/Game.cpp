#include "Game.h"


Game::Game(Shader* _shader, Camera _camera, CWindow _wnd, glm::vec3 _pos, glm::vec3 _size):
	pos(_pos),Playersize(_size)
{
	Wnd = _wnd;
	Gamecamera = _camera;
	this->Playershader = _shader;
	this->Buttershader = _shader;
}


void Game::PlayInit(Model* _model,Camera _camera)
{
	//Player->pos = glm::vec3(1.0f, 0, -5.0f);
	Player = _model;
	//if (this->pos.y!=0&&this->pos.y>=0)
	//{
	//	this->pos.y -= 0.05f;
	//}
	camera = _camera;
	Player->draw(this->Playershader,this->pos,_camera);
}

void Game::GetEnemyModel(Model* _melle_model, Model* _remote_model)
{
	Melleenemy.enemy = _melle_model;
	Melleenemy.type = "melle";
	Remote_enemy.enemy = _remote_model;
	Remote_enemy.type = "remote";
}


void Game::EnemyInit()
{
	ifstream data("Level_Data/EnemyData.txt");
	std::string line;
	GLuint enemycode;
	std::vector<std::vector<GLuint>> enemydata;

	while (getline( data,line))
	{
		std::istringstream mid(line);
		vector<GLuint> temp;
		while (mid>>enemycode)
			temp.push_back(enemycode);
		enemydata.push_back(temp);
	}
	
	unsigned int width = enemydata[0].size();
	unsigned int height = enemydata.size();
	unsigned int i;
	unsigned int j;
	while(enemy_numpos < enemyAmount)
	{
		for (i = 0; i < height; i++)
		{
			for (j = 0; j < width; j++)
			{
				//printf("%d\n", enemydata[i][j]);
				if (enemydata[i][j] == 1&& enemy_numpos < enemyAmount)
				{
					//printf("ssss");
					Model* melle_enemy = new Model("model/Fast Run.fbx");
					//Model* remote_enemy = new Model("model/Fast Run.fbx");
					Melleenemy.enemy = melle_enemy;
					Melleenemy.type = "melle";
					ALL_enemy.push_back(Melleenemy);

					ALL_enemy[enemy_numpos].enemy->enemyPos= glm::vec3(-3.0f + j, 0, 10.0f + i);
					//Enemy[enemy_numpos]->enemyPos = glm::vec3(-3.0f + j, 0, 10.0f+i);        //给位置赋上固定的初值
					//printf("%.2f\n", Enemy[numb]->enemyPos.x);
					//printf("%d--\n",numb);
					enemy_numpos++;
				}
				if (enemydata[i][j] == 2 && enemy_numpos < enemyAmount)
				{

				}
			}
		}
	}

}

void Game::EnemyDraw(Shader* enemyshader)
{
	for (unsigned int i = 0; i < ALL_enemy.size(); i++)
	{
		//Enemy[i]->enemydraw(enemyshader,this->pos);
		ALL_enemy[i].enemy->enemydraw(enemyshader, this->pos);
		
	}
}

void Game::ButterInit(Camera _camera, Shader* _shader)
{
	for (unsigned int i = 0; i < Butter.size(); i++)
	{
		Butter[i]->butterdraw(_shader,_camera.Yaw);
	}
	
}

void Game::WallInit(ObjModel* _model)
{
	Wall = _model;

	boxamount = 0;
	BoxMatrix = 0;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];

	ifstream data;		//读入关卡文件
	std::string line;
	GLuint levecode;
	std::vector<std::vector<GLuint>> LeveData;
	switch (Leve)
	{
	case 1:
		data.open("Level_Data/Level.txt");
		while (getline(data, line))
		{
			std::istringstream mid(line);
			vector<GLuint> temp;
			while (mid >> levecode)
				temp.push_back(levecode);
			LeveData.push_back(temp);
		}
		break;
	case 2:
		data.open("Level_Data/Leve2.txt");
		while (getline(data, line))
		{
			std::istringstream mid(line);
			vector<GLuint> temp;
			while (mid >> levecode)
				temp.push_back(levecode);
			LeveData.push_back(temp);
		}
		break;
	default:
		break;
	}
	unsigned int width = LeveData[0].size();
	unsigned int height = LeveData.size();
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			glm::mat4 wallModel;
			if (LeveData[i][j] == 1)
			{
				wallPos[boxamount] = glm::vec3(-4.0f + 1.5 * j, 0.0, -5.0f + 1.5 * i);	//关卡文件中大于0的位置渲染箱子

				wallModel = glm::translate(wallModel, wallPos[boxamount]);
				modelMatrices[BoxMatrix] = wallModel;
				boxamount++;
				BoxMatrix++;
			}
		}
	}
	amount = boxamount;		//记录渲染箱子数量，方便碰撞检测
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < Wall->meshes.size(); i++)
	{	
		unsigned int VAO = Wall->meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

}

void Game::obstaclesInit(ObjModel* _model)
{
	Obstacles = _model;
	obstacles_amount = 0;
	obstaclesMatrix = 0;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[obs_create_amount];

	ifstream data;		//读入关卡文件
	std::string line;
	GLuint levecode;
	std::vector<std::vector<GLuint>> LeveData;
	switch (Leve)
	{
	case 1:
		data.open("Level_Data/Level.txt");
		while (getline(data, line))
		{
			std::istringstream mid(line);
			vector<GLuint> temp;
			while (mid >> levecode)
				temp.push_back(levecode);
			LeveData.push_back(temp);
		}
		break;
	case 2:
		data.open("Level_Data/Leve2.txt");
		while (getline(data, line))
		{
			std::istringstream mid(line);
			vector<GLuint> temp;
			while (mid >> levecode)
				temp.push_back(levecode);
			LeveData.push_back(temp);
		}
		break;
	default:
		break;
	}
	unsigned int width = LeveData[0].size();
	unsigned int height = LeveData.size();
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < width; j++)
		{
			glm::mat4 obstaclesModel;
			if (LeveData[i][j] == 2)
			{
				obstaclesPos[obstacles_amount] = glm::vec3(-4.0f + 1.5 * j, 0.0, -5.0f + 1.5 * i);	//关卡文件中大于0的位置渲染箱子

				obstaclesModel = glm::translate(obstaclesModel, obstaclesPos[obstacles_amount]);
				modelMatrices[obstaclesMatrix] = obstaclesModel;
				obstacles_amount++;
				obstaclesMatrix++;
			}
		}
	}
	obs_create_amount = obstacles_amount;		//记录渲染箱子数量，方便碰撞检测
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, obs_create_amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);


	for (unsigned int i = 0; i < Obstacles->meshes.size(); i++)
	{
		unsigned int VAO = Obstacles->meshes[i].VAO;
		glBindVertexArray(VAO);
		// set attribute pointers for matrix (4 times vec4)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void Game::WallDraw(Shader* wallshader)
{
	wallshader->setuniform1i("texture_diffuse1", 1);

	for (unsigned int i = 0; i < Wall->meshes.size(); i++)
	{
		glBindVertexArray(Wall->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, Obstacles->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
}

void Game::ObstaclesDraw(Shader* obstaclesshader)
{
	obstaclesshader->setuniform1i("texture_diffuse1", 10);

	for (unsigned int i = 0; i < Obstacles->meshes.size(); i++)
	{
		glBindVertexArray(Obstacles->meshes[i].VAO);
		glDrawElementsInstanced(GL_TRIANGLES, Obstacles->meshes[i].indices.size(), GL_UNSIGNED_INT, 0, obs_create_amount);
		glBindVertexArray(0);
	}
}

void Game::GetDropModel(ObjModel* _model)
{
	Drop.push_back(_model);

}

void Game::DropInit(Shader* _shader)
{
	if (dropAmount!=0)
	{
		for (unsigned int i = 0; i < Drop.size(); i++)
		{
			Drop[i]->dropPos.x = DropPosx[i];
			Drop[i]->dropPos.z = DropPosz[i];
			Drop[i]->dropdraw(_shader);
			//printf("%.2lf  --%d\n", Drop[i]->dropPos.x,i);
		}
	}
}

void Game::Play_Enemy_gap()		//检测敌人与玩家之间的距离
{
	for (unsigned int i = 0; i < ALL_enemy.size(); i++)
	{
		ALL_enemy[i].enemy->enemy_yaw_z= this->pos.z - ALL_enemy[i].enemy->enemyPos.z;
		
		ALL_enemy[i].enemy->enemy_yaw_x = this->pos.x - ALL_enemy[i].enemy->enemyPos.x;
		ALL_enemy[i].enemy->enemy_shootButter = false;		//玩家未进入检测范围内，敌人不发射子弹
		//玩家是否进入检测半径（设置为3.0f）
		if (pow(ALL_enemy[i].enemy->enemy_yaw_z, 2) + pow(ALL_enemy[i].enemy->enemy_yaw_x, 2) < 16.0f)
		{
			ALL_enemy[i].enemy->enemyRotAngle = atan2(ALL_enemy[i].enemy->enemy_yaw_z, ALL_enemy[i].enemy->enemy_yaw_x) * 180 / 3.14159265;
			ALL_enemy[i].enemy->enemy_shootButter = true;
		}
		else {
			ALL_enemy[i].enemy->enemy_shootButter = false;
		}
	}
}
//子弹
void Game::EnemyButterInit(Shader* _shader)
{
	float yaw;//敌人旋转角度
	float pitch;
	for (unsigned int i = 0; i < EnemyButter.size(); i++)
	{
		
		EnemyButter[i]->enemybutterdraw(_shader, this->pos);
	}
}
//枪类武器
void Game::Melee_weaponsInit(ObjModel* _model, Shader* _shader)
{
	Melee_weapons = _model;
	Melee_weapons->melee_weapons_draw(_shader,this->pos,camera.Yaw);
}

void Game::NPCInit(ObjModel* _model, Shader* _shader)
{
	NPC = _model;
	NPC->NPCdraw(_shader);
}

void Game::buttercheck()
{
	for (unsigned int i = 0; i < Butter.size(); i++)
	{
		if (ALL_enemy.size()!=0)
		{
			unsigned int k = 0;
			for (k = 0; k < ALL_enemy.size(); k++)
			{
				if (enemyAmount != ALL_enemy.size())	//保证
				{
					enemyAmount = ALL_enemy.size();
				}

				if (Butter[i]->butterpos.x >= ALL_enemy[k].enemy->enemyPos.x - 0.143f && Butter[i]->butterpos.x <= ALL_enemy[k].enemy->enemyPos.x + 0.143f && Butter[i]->butterpos.z >= ALL_enemy[k].enemy->enemyPos.z - 0.481f && Butter[i]->butterpos.z <= ALL_enemy[k].enemy->enemyPos.z + 0.481f)		//0.143为敌人模型半径,0.781为模型Z方向上的半径.0.781视觉误差太大
				{
					//printf("kill\n");
					ALL_enemy[k].enemy->enemyBlood -= Butter[i]->butterAttack;
					//printf("%.2f_____%d\n", Enemy[k]->enemyBlood,k);
					if (ALL_enemy[k].enemy->enemyBlood <= 0)	//血量降低0一下死亡
					{
						dropAmount++;		//增加掉落物
						DropPosx.push_back(ALL_enemy[k].enemy->enemyPos.x);	//记录位置
						DropPosz.push_back(ALL_enemy[k].enemy->enemyPos.z);
						ALL_enemy.erase(ALL_enemy.begin() + k);
						//printf("%d---\n", enemyAmount);
						//enemyAmount--;
					}
					Butter.erase(Butter.begin() + i);
					break;	//如果当前子弹碰到敌人，子弹消失，跳出敌人for循环，检测下一个子弹
				}
			}
			
			if (k < enemyAmount)	//此if判断上一个for循环是否提前结束，若提前结束则说明当前子弹已经消失，无需进行下一个for循环。直接检测下一个子弹
			{
				continue;	//这里用enemyAmount做判断条件是因为如果上面子弹刚好打到最后一个敌人消失且敌人也消失，此时enemy.size()应该是等于k的
			}

		}
		for (unsigned int j = 0; j < amount; j++)
		{															//0.762为箱子的半径
			if (Butter[i]->butterpos.x >= wallPos[j].x - 0.762f && Butter[i]->butterpos.x <= wallPos[j].x + 0.762f && Butter[i]->butterpos.z >= wallPos[j].z - 0.762f && Butter[i]->butterpos.z <= wallPos[j].z + 0.762f)
			{
				//printf("Butter-WallBox\n");
				Butter.erase(Butter.begin() + i);
				break;
			}
		}

		if (Butter.size() == 0)		//当没有子弹时跳出循环，并且停止渲染
		{
			bo = false;
			break;
		}
	}
}

void Game::melee_weapons_check()
{
	for (unsigned int i = 0; i < ALL_enemy.size(); i++)
	{
		if (enemyAmount != ALL_enemy.size())
		{
			enemyAmount = ALL_enemy.size();
		}

		//printf("%.2f\n", Melee_weapons->butterpos.x);
		if (Melee_weapons->butterpos.x-0.7f<= ALL_enemy[i].enemy->enemyPos.x + 0.143f && Melee_weapons->butterpos.x - 0.7f >= ALL_enemy[i].enemy->enemyPos.x - 0.143f && Melee_weapons->butterpos.z-0.1f  <= ALL_enemy[i].enemy->enemyPos.z + 0.381f && Melee_weapons->butterpos.z-0.1f  >= ALL_enemy[i].enemy->enemyPos.z - 0.381f)
		{
			//Enemy[i]->enemyBlood -= 0.5f;		//近战武器攻击力0.5f,未设定变量
					//Enemy[i]->enemyPos.x -= 0.5f;			//被近战武器击中的敌人向后退。(总是向人物的对面后退,未写)
			
			if (ALL_enemy[i].enemy->enemyBlood <= 0)	//血量降低0一下死亡
			{
				dropAmount++;		//增加掉落物
				DropPosx.push_back(ALL_enemy[i].enemy->enemyPos.x);	//记录位置
				DropPosz.push_back(ALL_enemy[i].enemy->enemyPos.z);
				ALL_enemy.erase(ALL_enemy.begin() + i);
				//printf("%d---\n", enemyAmount);
				//enemyAmount--;
				continue;	//近战武器将当前敌人击败后，检测下一个
			}
		}
	}
}

bool Game::playercheck() //检测玩家与墙体,玩家于敌人的碰撞
{		
	for (unsigned int j = 0;j < amount; j++)
	{
		if (this->pos.x + Playersize.y >= wallPos[j].x - 0.762f && this->pos.x - Playersize.y <= wallPos[j].x + 0.762f && this->pos.z + Playersize.y >= wallPos[j].z - 0.762f && this->pos.z - Playersize.y <= wallPos[j].z + 0.762f)
		{
			return true;
		}
	}

	for (unsigned int i = 0; i < obs_create_amount; i++)
	{
		if (this->pos.x + Playersize.y >= obstaclesPos[i].x - 0.762f && this->pos.x - Playersize.y <= obstaclesPos[i].x + 0.762f && this->pos.z + Playersize.y >= obstaclesPos[i].z - 0.762f && this->pos.z - Playersize.y <= obstaclesPos[i].z + 0.762f)
		{
			return true;
		}
	}

	for (unsigned int p = 0; p < ALL_enemy.size(); p++)
	{
		if (this->pos.x + Playersize.y >= ALL_enemy[p].enemy->enemyPos.x - 0.143f && this->pos.x - Playersize.y <= ALL_enemy[p].enemy->enemyPos.x + 0.143f && this->pos.z + Playersize.y >= ALL_enemy[p].enemy->enemyPos.z && this->pos.z - Playersize.y <= ALL_enemy[p].enemy->enemyPos.z)		//敌人模型Z方向的大小不做计算了，视觉误差太大
		{
			return true;
		}
		 //printf("%.2f___%d\n", Enemy[p]->enemyPos.x,p);
	}

	return false;
}

void Game::enemyButtercheck()
{
	for (unsigned int i = 0; i < EnemyButter.size(); i++)
	{

		//子弹碰撞玩家
		if (this->pos.x + Playersize.y >= EnemyButter[i]->butterpos.x && this->pos.x - Playersize.y <= EnemyButter[i]->butterpos.x && this->pos.z + Playersize.y >= EnemyButter[i]->butterpos.z && this->pos.z - Playersize.y <= EnemyButter[i]->butterpos.z)
		{
			PlayLife--;
			EnemyButter.erase(EnemyButter.begin() + i);

			if (EnemyButter.size() == 0)
			{
				enemybo = false;
				break;
			}
			continue;	//若当前子弹碰到玩家则没必要检测墙体了,直接开始检测下一个子弹
		}
		
		for (unsigned int j = 0; j < amount; j++)	//子弹碰撞墙体
		{
			if (EnemyButter[i]->butterpos.x >= wallPos[j].x - 0.762f && EnemyButter[i]->butterpos.x <= wallPos[j].x + 0.762f && EnemyButter[i]->butterpos.z >= wallPos[j].z - 0.762f && EnemyButter[i]->butterpos.z <= wallPos[j].z + 0.762f)
			{
				//printf("Butter-WallBox\n");
				EnemyButter.erase(EnemyButter.begin() + i);
				break;
			}
		}
		
		if (EnemyButter.size()==0)
		{
			enemybo = false;
			break;
		}
	}
}

bool Game::enemy_Wall()
{
	//for (unsigned int j = 0; j < Wall->amount; j++)
	//{
	//	if (ALL_enemy[j].enemy->enemyPos.x + 0.143f >= Wall->wallPos[j].x - 0.762f && ALL_enemy[j].enemy->enemyPos.x - 0.143f <= Wall->wallPos[j].x + 0.762f && ALL_enemy[j].enemy->enemyPos.z+0.143f  >= Wall->wallPos[j].z - 0.762f && ALL_enemy[j].enemy->enemyPos.z-0.143f <= Wall->wallPos[j].z + 0.762f)
	//	{
	//		return true;
	//	}
	//}
	return false;
}

void Game::processInput(GLFWwindow* window)
{
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {

		this->pos.x -= 0.05f;
		Gamecamera.Position.x -= 0.05f;
		if (playercheck())
		{
			this->pos.x += 0.05f;
			Gamecamera.Position.x += 0.05f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {

		this->pos.x += 0.05f;
		Gamecamera.Position.x += 0.05f;
		if (playercheck())
		{
			this->pos.x -= 0.05f;
			Gamecamera.Position.x -= 0.05f;

		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ) {

		this->pos.z -= 0.05f;
		Gamecamera.Position.z -= 0.05f;
		if (playercheck())
		{
			this->pos.z += 0.05f;
			Gamecamera.Position.z += 0.05f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {

		this->pos.z += 0.05f;
		Gamecamera.Position.z += 0.05f;
		if (playercheck())
		{
			this->pos.z -= 0.05f;
			Gamecamera.Position.z -= 0.05f;
		}
	}
	if (glfwGetKey(window,GLFW_KEY_Q)==GLFW_PRESS)
	{
		
		angle += 2.0f*0.02f;

		Gamecamera.Position.x = cos(angle) * 3.0f;
		Gamecamera.Position.z = sin(angle) * 3.0f + 8.0f;
		Gamecamera.Yaw = cos(angle) * 3.0f + sin(angle)*3.0f;
		Gamecamera.UpdateCameraVectors();

		//Gamecamera.Forward = Gamecamera.ATview;
		//Gamecamera.ProcessMouseMovement(8.0f, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		angle -= 2.0f * 0.02f;
		Gamecamera.Position.x = cos(angle) * 2.0f;
		Gamecamera.Position.z = sin(angle) * 2.0f + 8.0f;
		Gamecamera.Forward -= Gamecamera.Position;
	}
	gameCameraPos = Gamecamera.Position;
}

	
	//if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
	//
	//	Enemy->enemyPos.x -= 0.05f;
	//	live++;
	//	if (enemy_Wall())
	//	{
	//		Enemy->enemyPos.x += 0.05f;
	//	}
	//}
	//if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
	//
	//	Enemy->enemyPos.x += 0.05f;
	//	live--;
	//	if (enemy_Wall())
	//	{
	//		Enemy->enemyPos.x -= 0.05f;
	//	}
	//}
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
	//
	//	Enemy->enemyPos.z -= 0.05f;
	//
	//	if (enemy_Wall())
	//	{
	//		Enemy->enemyPos.z += 0.05f;
	//	}
	//}
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
	//
	//	Enemy->enemyPos.z += 0.05f;
	//
	//	if (enemy_Wall())
	//	{
	//		Enemy->enemyPos.z -= 0.05f;
	//	}
	//}
