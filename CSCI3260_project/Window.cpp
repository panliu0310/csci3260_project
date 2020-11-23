#include "Window.h"

// Skybox
namespace Skybox
{
	std::vector<GLfloat> vertices =
	{
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
	};
	std::vector<const GLchar*> faces =
	{
		"Resources/texture/skybox/right.bmp",
		"Resources/texture/skybox/left.bmp",
		"Resources/texture/skybox/top.bmp",
		"Resources/texture/skybox/bottom.bmp",
		"Resources/texture/skybox/front.bmp",
		"Resources/texture/skybox/back.bmp"
	};
	GLuint vaoID{}, vboID{}, textureID{};
	int width = 0, height = 0, nr = 0;
}

// Camera
namespace Camera
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	float zoom = 1.0f;
}

// Clock
namespace Clock
{
	float time = 0.0f;
	float now, then = 0.0f;
}

// Constructor
Window::Window()
{
	std::cout << "Project started" << std::endl;
	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		this->status = -1;
	}

	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	/* Create a windowed mode window and its OpenGL context */
	this->window = glfwCreateWindow(this->width, this->height, "CSCI3260 Project", NULL, NULL);
	if (!this->window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		this->status = -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(this->window);

	/*register callback functions*/
	glfwSetFramebufferSizeCallback(this->window, Window::framebuffer_size_callback);
	glfwSetKeyCallback(this->window, Window::key_callback);
	glfwSetScrollCallback(this->window, Window::scroll_callback);
	glfwSetCursorPosCallback(this->window, Window::cursor_position_callback);
	glfwSetMouseButtonCallback(this->window, Window::mouse_button_callback);

	initializedGL();

	while (!glfwWindowShouldClose(this->window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(this->window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
}

// Get OpenGL info
void Window::get_OpenGL_info()
{
  const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

// Send data to OpenGL
void Window::sendDataToOpenGL()
{
	// Build skybox
	glGenVertexArrays(1, &Skybox::vaoID);
	glBindVertexArray(Skybox::vaoID);
	glGenBuffers(1, &Skybox::vboID);
	glBindBuffer(GL_ARRAY_BUFFER, Skybox::vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Skybox::vertices), &Skybox::vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glBindVertexArray(0);

	glGenTextures(1, &Skybox::textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::textureID);

	for (int i = 0; i < Skybox::faces.size(); i++) {
		unsigned char* data = stbi_load(Skybox::faces[i], &Skybox::width, &Skybox::height, &Skybox::nr, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, Skybox::width, Skybox::height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cannot load skybox texture" << std::endl;
			exit(1);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	std::cout << "Load skybox successfully!" << std::endl;
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Shaders
	this->createShader("Shaders/modelVertex.glsl", "Shaders/modelFragment.glsl");   // Model shader (0)
	this->createShader("Shaders/skyboxVertex.glsl", "Shaders/skyboxFragment.glsl"); // Skybox shader (1)

	// Models
	this->createModel("Resources/spacecraft.obj"); // Spacecraft (0)

	// Textures
	this->createTexture("Resources/texture/spacecraftTexture.bmp"); // Spacecraft (0)
}

// Initialize OpenGL
void Window::initializedGL(void)
{
	if (glewInit() != GLEW_OK) {
		std::cout << "GLEW not OK." << std::endl;
	}

	get_OpenGL_info();
	sendDataToOpenGL();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

// Paint OpenGL
void Window::paintGL(void)
{
	// Vectors and matrices
	glm::vec3 eyePosition(0.0f, 0.0f, 0.0f);
	glm::vec3 lightPosition(2.0f, 15.0f, 5.0f);

	glm::mat4 scaleMatrix_spacecraft = glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f));
	glm::mat4 rotationMatrix_spacecraft = glm::mat4(1.0f);
	glm::mat4 translateMatrix_spacecraft = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.0f, -15.0f));
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 skyboxViewMatrix = glm::mat4(glm::mat3(viewMatrix));
	glm::mat4 projectionMatrix = glm::perspective(45.0f, +8.0f / +6.0f, 1.0f, 1000.0f);

	// Update clock
	Clock::now = glfwGetTime();
	Clock::time = Clock::now - Clock::then;
	Clock::then = Clock::now;

	// Clear
	glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);

	// Model
	this->getShader(0).use();

	this->getShader(0).setVec3("eyePositionWorld", eyePosition);
	this->getShader(0).setVec3("dirlight.direction", lightPosition);
	this->getShader(0).setFloat("dirlight.intensity", 1.0f);
	this->getShader(0).setVec3("dirlight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	this->getShader(0).setVec3("dirlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	this->getShader(0).setVec3("dirlight.specular", glm::vec3(0.3f, 0.3f, 0.3f));
	this->getShader(0).setFloat("material.shininess", 32.0f);

	this->getShader(0).setMat4("scaleMatrix", scaleMatrix_spacecraft);
	this->getShader(0).setMat4("rotationMatrix", rotationMatrix_spacecraft);
	this->getShader(0).setMat4("translateMatrix", translateMatrix_spacecraft);
	this->getShader(0).setMat4("viewMatrix", viewMatrix);
	this->getShader(0).setMat4("projectionMatrix", projectionMatrix);

	this->getTexture(0).bind(0);
	this->getModel(0).draw();
	this->getTexture(0).unbind();

	// Skybox
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	this->getShader(1).use();

	this->getShader(1).setMat4("view", skyboxViewMatrix);
	this->getShader(1).setMat4("projection", projectionMatrix);

	glBindVertexArray(Skybox::vaoID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Unbind
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glUseProgram(0);
}

// Create shader
void Window::createShader(const char* vertexPath, const char* fragmentPath)
{
	this->shaders.push_back(Shader(vertexPath, fragmentPath));
}

// Remove shader
void Window::removeShader(unsigned int index)
{
	if (index > this->shaders.size()) {
		std::cout << "Invalid index" << std::endl;
		return;
	}
	this->shaders.erase(this->shaders.begin() + index);
}

// Create model
void Window::createModel(const char* objPath)
{
	this->models.push_back(Model(objPath));
}

// Remove model
void Window::removeModel(unsigned int index)
{
	if (index > this->models.size()) {
		std::cout << "Invalid index" << std::endl;
		return;
	}
	this->models.erase(this->models.begin() + index);
}

// Create texture
void Window::createTexture(const char* texturePath)
{
	this->textures.push_back(Texture(texturePath));
}

// Remove texture
void Window::removeTexture(unsigned int index)
{
	if (index > this->textures.size()) {
		std::cout << "Invalid index" << std::endl;
		return;
	}
	this->textures.erase(this->textures.begin() + index);
}

// Get status value
int Window::getStatus() {
	return this->status;
}

// Get shader
Shader Window::getShader(unsigned int index)
{
	return this->shaders[index];
}

// Get model
Model Window::getModel(unsigned int index)
{
	return this->models[index];
}

// Get texture
Texture Window::getTexture(unsigned int index)
{
	return this->textures[index];
}

// Frame buffer callback
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Mouse button callback
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}

// Cursor position callback
void Window::cursor_position_callback(GLFWwindow* window, double x, double y)
{

}

// Mouse scroll callback
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

// Keyboard key callback
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

}