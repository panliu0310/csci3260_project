#include "Window.h"

// Constructor
Window::Window()
{
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
	glClearColor(1.0f, 1.0f, 1.0f, 0.5f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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