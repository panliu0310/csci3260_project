#pragma once
#include "include.h"

using IV = PFNGLGETSHADERIVPROC;
using IL = PFNGLGETSHADERINFOLOGPROC;

class Shader
{
	// Constructor
	public:
		Shader(const char* vertexPath, const char* fragmentPath);

	// Variables
	private:
		GLuint id{};

	// Functions
	public:
		void use() const;
		void setMat4(const std::string& name, glm::mat4& value) const;
		void setVec4(const std::string& name, glm::vec4 value) const;
		void setVec3(const std::string& name, glm::vec3 value) const;
		void setFloat(const std::string& name, float value) const;
		void setInt(const std::string& name, int value) const;

	private:
		std::string readShaderCode(const char* fileName) const;
		bool checkShaderStatus(GLuint shaderID) const;
		bool checkProgramStatus(GLuint programID) const;
		bool checkStatus(GLuint objectID, IV objectPropertyGetterFunc, IL getInfoLogFunc, GLenum statusType) const;
};

