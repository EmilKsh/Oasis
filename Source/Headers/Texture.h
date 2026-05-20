#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

class Texture
{
public:
	Texture() = default;
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;

	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	bool LoadFromFile(const std::string& filePath, bool flipVertically = true);
	void Bind(GLenum textureUnit = GL_TEXTURE0) const;
	void Release();

	bool IsValid() const { return id != 0; }
	GLuint GetID() const { return id; }

private:
	GLuint id{};
};

#endif
