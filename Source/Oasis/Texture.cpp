#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include <utility>

Texture::~Texture()
{
	Release();
}

Texture::Texture(Texture&& other) noexcept
{
	id = std::exchange(other.id, 0);
}

Texture& Texture::operator=(Texture&& other) noexcept
{
	if (this != &other)
	{
		Release();
		id = std::exchange(other.id, 0);
	}

	return *this;
}

bool Texture::LoadFromFile(const std::string& filePath, bool flipVertically)
{
	Release();

	int width{}, height{}, channels{};
	stbi_set_flip_vertically_on_load(flipVertically);
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 0);
	if (!data)
	{
		std::cout << "Failed to load texture: " << filePath << std::endl;
		return false;
	}

	GLenum format = GL_RGB;
	if (channels == 1)
		format = GL_RED;
	else if (channels == 4)
		format = GL_RGBA;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	return true;
}

void Texture::Bind(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Release()
{
	if (id != 0)
	{
		glDeleteTextures(1, &id);
		id = 0;
	}
}
