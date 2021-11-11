#include "UI.h"

UI::UI(Shader* _bagshader, Shader* _equipshader)
{
	this->Bagshader = _bagshader;
	this->Equipshader = _bagshader;
}

void UI::GetInit()
{
	float UIvertices[] = {
	//	 0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right
	//	 0.5f, -0.5f, 0.0f,     1.0f, 0.0f, // bottom right
	//	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // bottom left
	//	
	//	-0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // bottom left
	//	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f,  // top left 
	//	0.5f,  0.5f, 0.0f,		1.0f, 1.0f, // top right

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UIvertices), &UIvertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

}

void UI::GameStartTextureInit()
{
	float GameStartTexturevertices[] = {

	 1.0f,  1.0f, 0.0f,    1.0f, 1.0f, // top right
	 1.0f, -1.0f, 0.0f,    1.0f, 0.0f, // bottom right
	 -1.0f,  1.0f, 0.0f,  0.0f, 1.0f , // top left 

	 -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,  // top left 
	1.0f, -1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	-1.0f, -1.0f, 0.0f,   0.0f, 0.0f // bottom left
	};
	glGenVertexArrays(1, &GameStartTextureVAO);
	glGenBuffers(1, &GameStartTextureVBO);
	glBindVertexArray(GameStartTextureVAO);

	glBindBuffer(GL_ARRAY_BUFFER, GameStartTextureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GameStartTexturevertices), &GameStartTexturevertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);
}
void UI::DrawGameStartTexture(Shader* _shader)
{
	_shader->use();

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0, 0, 0));
	glUniformMatrix4fv(glGetUniformLocation(_shader->ID, "modelMat"), 1, GL_FALSE, glm::value_ptr(model));

	_shader->setuniform1i("ourtexture", 10);

	glBindVertexArray(GameStartTextureVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void UI::Player_Condition(Shader* _shader)
{
	_shader->use();

	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-1.0f, 1.0f, 0));
	model = glm::scale(model, glm::vec3(1.0f, 0.5f, 1.0f));
	_shader->setuniform4fv("modelMat", model);
	_shader->setuniform1i("ourtexture", 7);
	//this->Bagshader->setunifor1f("UItexture[0].ID", 3);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void UI::GetBagUI()
{
	this->Bagshader->use();
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glm::mat4 model=glm::mat4(1.0f);
	this->Bagshader->setuniform4fv("modelMat",model);
	this->Bagshader->setuniform1i("ourtexture",3);
	//this->Bagshader->setunifor1f("UItexture[0].ID", 3);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void UI::GetGoings()
{
	this->Equipshader->use();
	
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(-0.28f, 0.25f, 0));
	model = glm::scale(model,glm::vec3(0.2,0.2,0.2));
	this->Equipshader->setuniform4fv("modelMat", model);

	this->Equipshader->setuniform1i("ourtexture", 4);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void UI::NPCconmmunication()
{
	this->Bagshader->use();
	//glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glm::mat4 model = glm::mat4(1.0f);
	this->Bagshader->setuniform4fv("modelMat", model);
	this->Bagshader->setuniform1i("ourtexture", 8);
	//this->Bagshader->setunifor1f("UItexture[0].ID", 3);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void UI::Delete()
{
	glDeleteVertexArrays(1, &this->VAO);
	glDeleteBuffers(1, &this->VBO);
}
