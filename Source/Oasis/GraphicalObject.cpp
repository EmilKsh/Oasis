#include"GraphicalObject.h"

std::map<Colors, glm::vec3> ColorValues = {
	{Colors::Amber, glm::vec3(1.0f, 0.75f, 0.0f) },
	{Colors::White, glm::vec3(1.0f, 1.0f, 1.0f) },
	{Colors::Blue, glm::vec3(0.0f, 0.102f, 1.0f) },
	{Colors::Black, glm::vec3(0.f)}
};

GraphicalObj::~GraphicalObj()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void GraphicalObj::BufferUpdate()
{
	glGenBuffers(1, &this->VBO);
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->EBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(float), static_cast<const void*>(vertexBuffer.data()), GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);

	if (!indexBuffer.empty())
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.size() * sizeof(int), static_cast<const void*>(indexBuffer.data()), GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
}


void GraphicalObj::VertexUpdate(vector<float>* vertices, vector<int>* indices)
{
	vertexBuffer.clear();

	for (float v : *vertices)
		vertexBuffer.push_back(v);

	if (indices != nullptr)
	{
		indexBuffer.clear();
		for (int i : *indices)
			indexBuffer.push_back(i);
	}
	else 
	{
		indexBuffer.clear();
	}

	this->BufferUpdate();
}

void GraphicalObj::DrawShape(Colors color)
{
	//this->BufferUpdate();
	if (shader)
	{
		shader->set3fv("myColor", ColorValues[color]);
	}	

	/*for (glm::vec3 trans: BoxLocations)
	{*/
	//transform(glm::vec3(1.f, 1.f, 1.f), glm::vec3(2.4f, -0.4f, -3.5f));

	glBindVertexArray(this->VAO);
	if (shader->HasTexture())
	{
		shader->setBool("hasTexture", true);
		glBindTexture(GL_TEXTURE_2D, shader->texture);
	
		if (!indexBuffer.empty())
		{	
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		shader->setBool("hasTexture", false);
		if (!indexBuffer.empty())
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glDrawElements(GL_TRIANGLES, indexBuffer.size(), GL_UNSIGNED_INT, 0);
		}
		else
			glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
	//}
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

glm::mat4 GraphicalObj::GetTransformMat() 
{
	return transformMatrix;
}