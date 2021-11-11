#include "Floor.h"

Floor::Floor(Shader* _shader)
{
	this->shader = _shader;
}


void Floor::FloorVertices()
{
	
	
}

void Floor::Init()
{
	
	float floorvertices[] = {
		15.0f, 0 , 15.0f,  25.0f, 0.0f,
		-15.0f,0 , 15.0f,  0.0f, 0.0f,
		-15.0f, 0, -15.0f,  0.0f, 25.0f,
		
		 15.0f,0,  15.0f,  25.0f, 0.0f,
		-15.0f, 0, -15.0f,  0.0f, 25.0f,
		 15.0f, 0, -15.0f,  25.0f, 25.0f
	};

	
	//std::vector<float> floorvertices;
	//float x = -50, z = -50;
	//for (int i = 0; i < row_num; i++) {
	//	x = -5;
	//	for (int j = 0; j < col_num; j++) {
	//		floorvertices.push_back(x);
	//		floorvertices.push_back(0);
	//		floorvertices.push_back(z);
	//		floorvertices.push_back(1.0f / col_num * j);
	//		floorvertices.push_back(1 - i * 1.0f / row_num);
	//		x += 0.1;
	//	}
	//	z += 0.1;
	//}
	//
	//std::vector<unsigned int> indicess;
	//for (int i = 1; i < row_num; i++) {
	//	for (int j = 1; j < col_num; j++) {
	//		indicess.push_back((i - 1) * col_num + j - 1);
	//		indicess.push_back((i - 1) * col_num + j);
	//		indicess.push_back(i * col_num + j - 1);
	//
	//		indicess.push_back(i * col_num + j - 1);
	//		indicess.push_back((i - 1) * col_num + j);
	//		indicess.push_back(i * col_num + j);
	//	}
	//}


	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindVertexArray(floorVAO);

	//glGenBuffers(1, &floorEBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, floorEBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indicess[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorvertices), &floorvertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

}

void Floor::NoiseInit()
{
	float noisevertices[] = {
		1.0f, 0 , 1.0f,  1.0f, 0.0f,
		-1.0f,0 , 1.0f,  0.0f, 0.0f,
		-1.0f, 0, -1.0f,  0.0f, 1.0f,

		 1.0f,0,  1.0f,  1.0f, 0.0f,
		-1.0f, 0, -1.0f,  0.0f, 1.0f,
		 1.0f, 0, -1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &noiseVAO);
	glGenBuffers(1, &noiseVBO);
	glBindVertexArray(noiseVAO);

	glBindBuffer(GL_ARRAY_BUFFER, noiseVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(noisevertices), &noisevertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void Floor::TransDoorInit()
{
	float TransDoorvertices[] = {
		1.0f, 0 , 1.0f,  1.0f, 0.0f,
		-1.0f,0 , 1.0f,  0.0f, 0.0f,
		-1.0f, 0, -1.0f,  0.0f, 1.0f,

		 1.0f,0,  1.0f,  1.0f, 0.0f,
		-1.0f, 0, -1.0f,  0.0f, 1.0f,
		 1.0f, 0, -1.0f,  1.0f, 1.0f
	};

	glGenVertexArrays(1, &TransDoorVAO);
	glGenBuffers(1, &TransDoorVBO);
	glBindVertexArray(TransDoorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, TransDoorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TransDoorvertices), &TransDoorvertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}

void Floor::draw()
{
	this->shader->use();

	glm::mat4 model;
	model = glm::translate(model,glm::vec3(0,0,0));
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));
	
	this->shader->setuniform1i("ourtexture", 6);

	glBindVertexArray(floorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, (row_num - 1) * (col_num - 1) * 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Floor::drawNoise()
{
	this->shader->use();
	unsigned int cTime = 0;
	srand((int)time(0));
	cTime = rand() % 8;
	float timeValue = glfwGetTime();
	float scaleValue = sin(timeValue) / 2.0f + 0.5f;
	//printf("%.2lf\n",scaleValue);
	glm::mat4 model;

	model = glm::translate(model, NoisePos);
	model = glm::scale(model,glm::vec3(0.8f,0.8f,0.8f));
	
	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	this->shader->setuniform1i("ourtexture", TextureID);

	glBindVertexArray(noiseVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	TextureGap --;
	if (TextureGap<=0)
	{
		TextureID++;
		TextureGap = 2.0f;
	}
	if (TextureID >133)
	{
		TextureID = 11;
	}
}

void Floor::drawTransDoor()
{
	this->shader->use();

	glm::mat4 model;

	model = glm::translate(model, TransDoorPos);
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	this->shader->setuniform1i("ourtexture", DoorTextureID);

	glBindVertexArray(TransDoorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	DoorTextureGap --;
	if (DoorTextureGap <= 0)
	{
		DoorTextureID++;
		DoorTextureGap = 2.0f;
	}
	if (DoorTextureID > 133)
	{
		DoorTextureID = 11;
	}
}

void Floor::drawDianJi()
{
	this->shader->use();

	glm::mat4 model;

	model = glm::translate(model, DianJiTransPos);
	model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

	glUniformMatrix4fv(glGetUniformLocation(shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	this->shader->setuniform1i("ourtexture", DianjiTextureID);

	glBindVertexArray(TransDoorVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

	DianJiTextureGap --;
	if (DianJiTextureGap <= 0)
	{
		DianJiTransPos.y += 0.02f;
		DianjiTextureID++;
		DianJiTextureGap = 2.0f;
	}
	if (DianjiTextureID > 186)
	{
		DianJiTransPos.y = 0.01f;
		DianjiTextureID = 134;
	}
}
