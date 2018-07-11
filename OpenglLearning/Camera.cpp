#include "Camera.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Camera::Camera(glm::vec3 pos,glm::vec3 worldUp,float yaw, float pitch)
{


	this->fov = 45.0f;
	this->near = 0.1f;
	this->far = 100.0f;

	this->front = glm::vec3(0.0f, 0.0f, -1.0f);
	this->pos = pos;
	this->worldUp = worldUp;
	this->yaw = yaw;
	this->pitch = pitch;

	this->updateCameraVectors();
}

void Camera::SetPosition(glm::vec3 pos)
{

}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(pos, pos + front,up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	return glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, near, far);
}

void Camera::ProcessKeyBoard(Camera_Movement move, float deltaTime)
{
	float velocity = moveSpeed * deltaTime;
	if(Camera_Movement::FORWARD == move)
	{
		pos += front * velocity;
	}
	else if(Camera_Movement::BACKWARD == move)
	{
		pos -= front * velocity;
	}
	else if(Camera_Movement::LEFT == move)
	{
		pos -= right * velocity;
	}
	else if (Camera_Movement::RIGHT == move)
	{
		pos += right * velocity;
	}
}

void Camera::ProcessMouse(float mouseX, float mouseY, float deltaTime)
{
	mouseX *= mouseSensitivity;
	mouseY *= mouseSensitivity;

	yaw += mouseX;
	pitch += mouseY;

	if(pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if(pitch < -89.0f)
	{
		pitch = -89.0f;
	}
	updateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::updateCameraVectors()
{
	glm::vec3 forward;
	forward.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward.y = sin(glm::radians(pitch));
	forward.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = glm::normalize(forward);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}
