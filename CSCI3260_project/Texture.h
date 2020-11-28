#pragma once
#include "include.h"

class Texture
{
	// Constructor
	public:
		Texture(const char* texturePath);

	// Variables
	private:
		GLuint id{};
		int width = 0, height = 0, bpp = 0;

	// Functions
	public:
		void bind(GLuint slot) const;
		void unbind() const;
};