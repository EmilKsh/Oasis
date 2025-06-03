#pragma once
#include<vector>
#include<glm/glm.hpp>

class Light
{
public:
	glm::vec3 position{ 3.f, 2.f, -2.f };
	float intensity{ 10.f };
};

