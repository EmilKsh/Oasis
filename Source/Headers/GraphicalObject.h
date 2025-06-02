#ifndef Graphics_H
#define Graphics_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string_view>
#include<vector>
#include"Shader.h"

using std::vector;

struct Colors
{
	glm::vec3 Amber{ glm::vec3(1.0f, 0.75f, 0.0f) };
	glm::vec3 White{ glm::vec3(1.0f, 1.0f, 1.0f) };
	glm::vec3 MayiBleu{ 0.0f, 0.102f, 1.0f };
};


class GraphicalObj {

public:
	~GraphicalObj();
	void SetShader(Shader* shader);
	void VertexUpdate(vector<float>* vertices, vector<int>* indices = nullptr);
	void BufferUpdate();
	void DrawShape(glm::vec3 color = glm::vec3(1.0f,1.0f,1.0f));
	void transform(const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), 
				   const glm::vec3& translate = glm::vec3(0.0f, 0.0f, 0.0f), 
				   const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	Shader* getShader();
	glm::mat4 GetTransformMat();

	vector<float> vertexBuffer = {
		// Postition             Texture Coords
		0.5f, 0.5f, 0.0f,        1.0f, 1.0f,          //top right
		0.5f, -0.5f, 0.0f,       1.0f, 0.0f,          //bottom right
		-0.5f, -0.5f, 0.0f,      0.0f, 0.0f,          //bottom left
		-0.5f, 0.5f, 0.0f,       0.0f, 1.0f           //top left
	};

	vector<int> indexBuffer = {
		0, 1, 3,
		1, 2, 3,
	};

private:
	Shader* shader;
	GLuint VBO{}, VAO{}, EBO{};
	const char* texturePath;
	glm::mat4 transformMatrix{glm::mat4(1.f)};
};
#endif // !GUI_H4