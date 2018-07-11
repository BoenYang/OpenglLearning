#include "Transform.h"
#include <glm/gtc/matrix_transform.inl>


Transform::Transform()
{
	position = glm::vec3(0, 0, 0);
	eulersRoataion = glm::vec3(0, 0, 0);
	localScale = glm::vec3(1, 1, 1);
}


Transform::~Transform()
{
}

glm::mat4 Transform::GetModelMatrix()
{
	glm::mat4 model;
	model = glm::translate(model, position);
	model = glm::rotate(model, eulersRoataion.x, glm::vec3(1,0,0));
	model = glm::rotate(model, eulersRoataion.y, glm::vec3(0,1,0));
	model = glm::rotate(model, eulersRoataion.z, glm::vec3(0,0,1));
	model = glm::scale(model, localScale);
	return model;
}
