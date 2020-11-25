#define STB_IMAGE_IMPLEMENTATION
#define DEBUG_MODE false
#include "Texture.h"

// Constructor
Texture::Texture(const char* texturePath)
{
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath, &this->width, &this->height, &this->bpp, 0);
	GLenum format = 3;
	switch (this->bpp) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	}

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, format, this->width, this->height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		stbi_image_free(data);
	} else {
		std::cout << "Failed to load texture: " << texturePath << std::endl;
		exit(1);
	}

	if (DEBUG_MODE) { std::cout << "Load " << texturePath << " successfully!" << std::endl; }
	glBindTexture(GL_TEXTURE_2D, 0);
}

// Bind texture
void Texture::bind(GLuint slot = 0) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, this->id);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}