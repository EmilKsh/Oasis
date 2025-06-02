#include "Camera.h"

glm::mat4 Camera::getView() 
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

glm::mat4 Camera::GetProjection()
{
	return glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
}

void Camera::setTransform(glm::vec3 pos, glm::vec3 front, glm::vec3 up) 
{
	cameraPos = pos;
	cameraFront = front;
	cameraUp = up;
}