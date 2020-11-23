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
		Model(const char* objPath, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, unsigned int txtr);

	// Variables
	private:
		GLuint vaoID{}, vboID{}, eboID{};
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	private:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		unsigned int texture = 0;

	// Functions
	public:
		void draw();

	// Getters
	public:
		glm::vec3 getPosition();
		glm::vec3 getRotation();
		glm::vec3 getScale();
		unsigned int getTexture();

	// Setters
	public:
		void setPosition(glm::vec3 pos);
		void setRotation(glm::vec3 rot);
		void setScale(glm::vec3 scl);
		void setTexture(unsigned int txtr);
};

