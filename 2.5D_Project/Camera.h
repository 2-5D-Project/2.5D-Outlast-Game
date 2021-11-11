#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup);
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;
	float speedz = 0.0f;
	float speedx = 0;
	float speedy = 0;
	float mouse_posx;
	float mouse_posy;

	glm::vec3 ATview;
	float Pitch;
	float Yaw;
	glm::mat4 GetViewMatrix();
	void ProcessMouseMovement(float delatx, float delaty);
	void updatecameraPos();
	void UpdateCameraVectors();

private:
	
};

