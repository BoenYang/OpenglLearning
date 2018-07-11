#pragma once
#include <glm/glm.hpp>

class Transform
{
public:
	Transform();
	~Transform();
	glm::vec3 position;
	glm::vec3 eulersRoataion;
	glm::vec3 localScale;
	glm::mat4 GetModelMatrix();
};

