#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera
{
public:
	float fov = 45.0f;
	float near = 0.1f;
	float far = 100.0f;
	float yaw;
	float pitch;
	glm::vec3 pos;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 front;
	glm::vec3 worldUp;
	float moveSpeed = 2.5f;
	float mouseSensitivity = 0.2f;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

	void SetPosition(glm::vec3 pos);

	glm::mat4 GetViewMatrix();

	glm::mat4 GetProjectionMatrix();

	void ProcessKeyBoard(Camera_Movement move,float deltaTime);

	void ProcessMouse(float mouseX, float mouseY, float deltaTime);

	~Camera();
private:
	void updateCameraVectors();
};

