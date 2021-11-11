#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
Camera::Camera()
{
}
Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup) {
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Right, Forward));

}

Camera::Camera(glm::vec3 position, float pitch, float yaw, glm::vec3 worldup) {
	Position = position;
	WorldUp = worldup;
	Pitch = pitch;
	Yaw = yaw;
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Forward, Right));
	ATview = Position + Forward;

}
glm::mat4 Camera::GetViewMatrix() {
	return glm::lookAt(Position, Position + Forward, WorldUp);
}
void Camera::UpdateCameraVectors() {
	Forward.x = glm::cos(Pitch) * glm::sin(Yaw);
	Forward.y = glm::sin(Pitch);
	Forward.z = glm::cos(Pitch) * glm::cos(Yaw);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Forward, Right));
}
void Camera::ProcessMouseMovement(float delatx, float delaty) {
	Pitch -= delaty * 0.002f;
	Yaw -= delatx * 0.002f;
	//if (Pitch > 89.0f)
	//	Pitch = 89.0f;
	//if (Pitch < -89.0f)
	//	Pitch = -89.0f;

	UpdateCameraVectors();

}
void Camera::updatecameraPos() {
	if (speedx != 0)
		Position -= Right * speedx * 0.05f;	//速度与玩家速度一致
	if (speedz != 0)
		Position += Forward * speedz * 0.05f;
	if (speedy != 0)
		Position += Up * speedy * 0.05f;
}