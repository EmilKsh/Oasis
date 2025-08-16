
#ifndef SHADER_H
#define SHADER_H

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<string>
#include<istream>
#include<fstream>
#include<sstream>
#include<iostream>
#include<filesystem>

using std::string;

class Shader
{
public:
	unsigned int ID;
	Shader();
	void Init(string shaderFilePath);
	void use();
	void AddTexture(const char* filePath);
	void setBool(const string &name, bool value) const;
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
	void set3fv(const string& name, glm::vec3 value) const;
	void set4mat(const string& name, glm::mat4 mat);
	bool HasTexture();
	unsigned int texture{};

private:
	bool HasTextureFlag{ false };
	const char* vShaderCode;
	const char* fShaderCode;
	int vertex, fragment;
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	void checkCompileErrors(int shader, string type);
};

#endif // !H_SHADER