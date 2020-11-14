#include "Shader.h"

// Constructor
Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* vCode;
	std::string temp = readShaderCode(vertexPath);
	vCode = temp.c_str();
	glShaderSource(vertexShaderID, 1, &vCode, NULL);
	glCompileShader(vertexShaderID);

	const GLchar* fCode;
	temp = readShaderCode(fragmentPath);
	fCode = temp.c_str();
	glShaderSource(fragmentShaderID, 1, &fCode, NULL);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID)) return;

	this->id = glCreateProgram();
	glAttachShader(this->id, vertexShaderID);
	glAttachShader(this->id, fragmentShaderID);
	glLinkProgram(this->id);

	if (!checkProgramStatus(this->id)) return;
		
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(0);
}

// Use shader
void Shader::use() const
{
	glUseProgram(this->id);
}

// Set mat4 data
void Shader::setMat4(const std::string& name, glm::mat4& value) const
{
	unsigned int transformLoc = glGetUniformLocation(this->id, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}

// Set vec4 data
void Shader::setVec4(const std::string& name, glm::vec4 value) const
{
	glUniform4fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}

// Set vec3 data
void Shader::setVec3(const std::string& name, glm::vec3 value) const
{
	glUniform3fv(glGetUniformLocation(this->id, name.c_str()), 1, &value[0]);
}

// Set float data
void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

// Set int data
void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

// Read shader code
std::string Shader::readShaderCode(const char* fileName) const
{
	std::ifstream myInput(fileName);
	if (!myInput.good()) {
		std::cout << "File failed to load..." << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(myInput),
		std::istreambuf_iterator<char>()
	);
}

// Check shader status
bool Shader::checkShaderStatus(GLuint shaderID) const
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

// Check program status
bool Shader::checkProgramStatus(GLuint programID) const
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

// Check status
bool Shader::checkStatus(GLuint objectID, PFNGLGETSHADERIVPROC objectPropertyGetterFunc, PFNGLGETSHADERINFOLOGPROC getInfoLogFunc, GLenum statusType) const
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE) {
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}