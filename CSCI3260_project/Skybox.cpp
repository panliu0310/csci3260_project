#include "Skybox.h"

// Constructor
Skybox::Skybox()
{
	/*
	glGenVertexArrays(1, &this->vao);
	glGenBuffers(1, &this->vbo);
	glBindVertexArray(this->vao);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(this->vertices), &this->vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);

	this->faces.push_back("Resources/texture/skybox/right.bmp");
	this->faces.push_back("Resources/texture/skybox/left.bmp");
	this->faces.push_back("Resources/texture/skybox/bottom.bmp");
	this->faces.push_back("Resources/texture/skybox/top.bmp");
	this->faces.push_back("Resources/texture/skybox/back.bmp");
	this->faces.push_back("Resources/texture/skybox/front.bmp");

	glGenTextures(1, &this->id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);
	
	stbi_set_flip_vertically_on_load(true);
	for (int i = 0; i < this->faces.size(); i++) {
		this->image = stbi_load(this->faces[i], &this->width, &this->height, &this->bpp, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, this->width, this->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->image);
		stbi_image_free(this->image);
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	*/
}