#ifndef Graphics_H
#define Graphics_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<string_view>
#include<vector>
#include"Shader.h"
#include"Texture.h"
#include<map>

using std::vector;

enum Colors{Black, White, Amber, Blue};

extern std::map<Colors, glm::vec3> ColorValues;


class GraphicalObj {

public:
	GraphicalObj() = default;
	~GraphicalObj();

	GraphicalObj(const GraphicalObj&) = delete;
	GraphicalObj& operator=(const GraphicalObj&) = delete;

	GraphicalObj(GraphicalObj&& other) noexcept;
	GraphicalObj& operator=(GraphicalObj&& other) noexcept;

	void SetShader(Shader* shader);
	void SetShader(Shader& shader);
	void SetTexture(Texture* texture);
	void SetTexture(Texture& texture);
	void VertexUpdate(const vector<float>& vertices, const vector<int>* indices = nullptr);
	void VertexUpdate(vector<float>* vertices, vector<int>* indices = nullptr);
	void BufferUpdate();
	void DrawShape(Colors color = Colors::White);
	void transform(const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f), 
				   const glm::vec3& translate = glm::vec3(0.0f, 0.0f, 0.0f), 
				   const glm::vec3& rotation = glm::vec3(0.0f, 0.0f, 0.0f));
	Shader* getShader();
	glm::mat4 GetTransformMat();

	vector<float> vertexBuffer = {
		// Postition             Texture Coords		Vertex Normal
		0.5f, 0.5f, 0.0f,        1.0f, 1.0f,		0.f, 1.f, 0.f,       //top right
		0.5f, -0.5f, 0.0f,       1.0f, 0.0f,		0.f, 1.f, 0.f,       //bottom right
		-0.5f, -0.5f, 0.0f,      0.0f, 0.0f,		0.f, 1.f, 0.f,       //bottom left
		-0.5f, 0.5f, 0.0f,       0.0f, 1.0f,		0.f, 1.f, 0.f        //top left
	};

	vector<int> indexBuffer = {
		0, 1, 3,
		1, 2, 3,
	};

private:
	void ReleaseBuffers();

	Shader* shader{};
	Texture* texture{};
	GLuint VBO{}, VAO{}, EBO{};
	GLsizei vertexCount{};
	GLsizei indexCount{};
	glm::mat4 transformMatrix{glm::mat4(1.f)};
};
#endif // !GUI_H4
