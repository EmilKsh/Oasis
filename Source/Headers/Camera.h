#pragma once
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

class Camera
{
public:

	glm::mat4 getView();
	glm::mat4 GetProjection();
	void setTransform(glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
	
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

private:
	glm::mat4 view, projection; 
	float pitch{ 0.0f }, roll{ 0.0f }, yaw{ -90.0f };
};

