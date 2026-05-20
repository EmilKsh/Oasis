#include"GraphicalObject.h"

#include <utility>

std::map<Colors, glm::vec3> ColorValues = {
	{Colors::Amber, glm::vec3(1.0f, 0.75f, 0.0f) },
	{Colors::White, glm::vec3(1.0f, 1.0f, 1.0f) },
	{Colors::Blue, glm::vec3(0.0f, 0.102f, 1.0f) },
	{Colors::Black, glm::vec3(0.f)}
};

GraphicalObj::~GraphicalObj()
{
	ReleaseBuffers();
}

GraphicalObj::GraphicalObj(GraphicalObj&& other) noexcept
{
	vertexBuffer = std::move(other.vertexBuffer);
	indexBuffer = std::move(other.indexBuffer);
	shader = std::exchange(other.shader, nullptr);
	texture = std::exchange(other.texture, nullptr);
	VBO = std::exchange(other.VBO, 0);
	VAO = std::exchange(other.VAO, 0);
	EBO = std::exchange(other.EBO, 0);
	vertexCount = std::exchange(other.vertexCount, 0);
	indexCount = std::exchange(other.indexCount, 0);
	transformMatrix = other.transformMatrix;
}

GraphicalObj& GraphicalObj::operator=(GraphicalObj&& other) noexcept
{
	if (this != &other)
	{
		ReleaseBuffers();
		vertexBuffer = std::move(other.vertexBuffer);
		indexBuffer = std::move(other.indexBuffer);
		shader = std::exchange(other.shader, nullptr);
		texture = std::exchange(other.texture, nullptr);
		VBO = std::exchange(other.VBO, 0);
		VAO = std::exchange(other.VAO, 0);
		EBO = std::exchange(other.EBO, 0);
		vertexCount = std::exchange(other.vertexCount, 0);
		indexCount = std::exchange(other.indexCount, 0);
		transformMatrix = other.transformMatrix;
	}

	return *this;
}

void GraphicalObj::BufferUpdate()
{
	ReleaseBuffers();

	if (vertexBuffer.empty())
		return;

	vertexCount = static_cast<GLsizei>(vertexBuffer.size() / 8);
	indexCount = static_cast<GLsizei>(indexBuffer.size());

	glGenBuffers(1, &this->VBO);
	glGenVertexArrays(1, &this->VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), vertexBuffer.data(), GL_STATIC_DRAW);

	if (!indexBuffer.empty())
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(int), indexBuffer.data(), GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


void GraphicalObj::VertexUpdate(const vector<float>& vertices, const vector<int>* indices)
{
	vertexBuffer = vertices;

	if (indices != nullptr)
		indexBuffer = *indices;
	else 
		indexBuffer.clear();

	this->BufferUpdate();
}

void GraphicalObj::VertexUpdate(vector<float>* vertices, vector<int>* indices)
{
	if (vertices == nullptr)
	{
		vertexBuffer.clear();
		indexBuffer.clear();
		ReleaseBuffers();
		return;
	}

	VertexUpdate(*vertices, indices);
}

void GraphicalObj::DrawShape(Colors color)
{
	if (shader == nullptr || VAO == 0)
		return;

	shader->set3fv("myColor", ColorValues[color]);
	shader->setBool("hasTexture", texture != nullptr && texture->IsValid());
	if (texture != nullptr && texture->IsValid())
	{
		texture->Bind();
	}

	glBindVertexArray(VAO);
	if (indexCount > 0)
	{
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	}
	else if (vertexCount > 0)
	{
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void GraphicalObj::transform(const glm::vec3& scale, const glm::vec3& translate, const glm::vec3& rotation)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, translate);
	model = glm::scale(model, scale);
	if (glm::length(rotation))
		model = glm::rotate(model, glm::length(rotation), glm::normalize(rotation));

	transformMatrix = model;
}

Shader* GraphicalObj::getShader()
{
	return this->shader;
}

void GraphicalObj::SetShader(Shader* shader)
{
	this->shader = shader;
}

void GraphicalObj::SetShader(Shader& shader)
{
	this->shader = &shader;
}

void GraphicalObj::SetTexture(Texture* texture)
{
	this->texture = texture;
}

void GraphicalObj::SetTexture(Texture& texture)
{
	this->texture = &texture;
}

glm::mat4 GraphicalObj::GetTransformMat() 
{
	return transformMatrix;
}

void GraphicalObj::ReleaseBuffers()
{
	if (VBO != 0)
	{
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}
	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	vertexCount = 0;
	indexCount = 0;
}
