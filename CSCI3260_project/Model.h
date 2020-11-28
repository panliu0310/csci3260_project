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
		Model(const char* objPath, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, uint txtr);

	// Variables
	private:
		GLuint vaoID{}, vboID{}, eboID{};
		std::vector<Vertex> vertices;
		std::vector<uint> indices;

	private:
		glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
		uint texture = 0;
		float alpha = 1.0f;

	// Functions
	public:
		void draw();
		static float dist(glm::vec3 posA, glm::vec3 posB);

	// Getters
	public:
		glm::vec3 getPosition();
		glm::vec3 getRotation();
		glm::vec3 getScale();
		uint getTexture();
		float getAlpha();

	// Setters
	public:
		void setPosition(glm::vec3 pos);
		void setRotation(glm::vec3 rot);
		void setScale(glm::vec3 scl);
		void setTexture(uint txtr);
		void setAlpha(float alpha);
};