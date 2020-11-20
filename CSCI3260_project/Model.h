#pragma once
#include "include.h"
#include "Texture.h"

struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

class Model
{
	// Constructor
	public:
		Model(const char* objPath);

	// Variables
	private:
		GLuint vaoID{}, vboID{}, eboID{};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	// Functions
	public:
		void draw() const;
};

