#define CHECKPOINTS 5
#include "Window.h"

// Skybox
namespace Skybox
{
	GLfloat vertices[108] =
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
	GLuint vaoID, vboID, textureID;
}

// Camera
namespace Camera
{
	glm::vec3 position = glm::vec3(0.0f, 6.0f, 15.0f);
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
	float zoom = 1.0f;
}

// Spacecraft
namespace Spacecraft
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 180.0f, 0.0f);
	int tasks = 0;
}

// Clock
namespace Clock
{
	double time = 0;
	double now, then = 0;
	double delta = 0;
}

// Controls
namespace Controls
{
	bool leftMouse = false;
	double posX = 0, posY = 0;
}

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
	// Shaders
	this->createShader("Shaders/modelVertex.glsl", "Shaders/modelFragment.glsl");   // Model shader (0)
	this->createShader("Shaders/skyboxVertex.glsl", "Shaders/skyboxFragment.glsl"); // Skybox shader (1)

	// Build skybox
	glGenVertexArrays(1, &Skybox::vaoID);
	glGenBuffers(1, &Skybox::vboID);
	glBindVertexArray(Skybox::vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, Skybox::vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Skybox::vertices), &Skybox::vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glGenTextures(1, &Skybox::textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::textureID);

	int w, h, nr;
	for (int i = 0; i < Skybox::faces.size(); i++) {
		unsigned char* data = stbi_load(Skybox::faces[i], &w, &h, &nr, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

	// Models
	this->createModel("Resources/spacecraft.obj", Spacecraft::position, Spacecraft::rotation, glm::vec3(0.01f, 0.01f, 0.01f), 0);								     // Spacecraft
	this->createModel("Resources/planet.obj", glm::vec3(0.0f, 0.0f, -280.0f - 40.0f * CHECKPOINTS), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(20.0f, 20.0f, 20.0f), 4); // Planet
	for (int i = 1; i <= CHECKPOINTS; i++) {
		this->createAlien(glm::vec3(glm::linearRand(-20.0f, 20.0f), 0.0f, -40.0f * i), i % 2);																		 // Aliens
		Spacecraft::tasks += 2;
	}
	for (int j = 0; j <= 300; j++) {
		this->createRock(j);																																		 // Rocks
	}

	// Textures
	this->createTexture("Resources/texture/spacecraftTexture.bmp");             // Spacecraft (0)
	this->createTexture("Resources/texture/leisure_spacecraftTexture.bmp");     // Leisure spacecraft (1)
	this->createTexture("Resources/texture/alienTexture.bmp");                  // Alien (2)
	this->createTexture("Resources/texture/colorful_alien_vehicleTexture.bmp"); // Colourful alien (3)
	this->createTexture("Resources/texture/planetTexture.bmp");                 // Planet (4)
	this->createTexture("Resources/texture/planetNormal.bmp");                  // Planet normal (5)
	this->createTexture("Resources/texture/rockTexture.bmp");                   // Rock (6)
	this->createTexture("Resources/texture/chickenTexture.bmp");                // Chicken (7)
	this->createTexture("Resources/texture/rockNormal.png");                    // Rock normal (8)
	this->createTexture("Resources/texture/yellow.png");						// Banana (9)
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
}

// Paint OpenGL
void Window::paintGL(void)
{
	// Update spacecraft
	this->models[0].setPosition(Spacecraft::position);
	this->models[0].setRotation(Spacecraft::rotation);

	// Calculate local directions and camera
	glm::mat4 worldMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(this->models[0].getRotation().x), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(this->models[0].getRotation().y - 180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(this->models[0].getRotation().z), glm::vec3(0, 0, 1));
	glm::vec3 front = glm::vec3(worldMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

	Camera::position = this->models[0].getPosition() + glm::vec3(worldMatrix * glm::vec4(glm::vec3(0.0f, 6.0f, 15.0f), 1.0f));
	Camera::direction = front;

	// Spaceship-dependent matrices
	glm::mat4 viewMatrix = glm::lookAt(Camera::position, Camera::position + Camera::direction, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 projectionMatrix = glm::perspective(45.0f / Camera::zoom, (float)this->width / (float)this->height, 0.1f, 1000.0f);
	glm::mat4 skyboxViewMatrix = glm::mat4(glm::mat3(viewMatrix));

	// Update clock
	Clock::now = glfwGetTime();
	Clock::delta = Clock::now - Clock::then;
	Clock::time += Clock::delta;
	Clock::then = Clock::now;

	// Clear
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Model
	this->shaders[0].use();

	this->shaders[0].setVec3("eyePositionWorld", Camera::position);

	this->shaders[0].setVec3("dirlight.direction", glm::vec3(2.0f, 15.0f, 5.0f));
	this->shaders[0].setFloat("dirlight.intensity", 1.0f);
	this->shaders[0].setVec3("dirlight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
	this->shaders[0].setVec3("dirlight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
	this->shaders[0].setVec3("dirlight.specular", glm::vec3(0.3f, 0.3f, 0.3f));


	this->shaders[0].setVec3("pointlight.position", glm::vec3(0.0f, 5.0f, 5.0f));
	this->shaders[0].setFloat("pointlight.constant", 1.0f);
	this->shaders[0].setFloat("pointlight.linear", 0.045f);
	this->shaders[0].setFloat("pointlight.quadratic", 0.001f);
	this->shaders[0].setVec3("pointlight.ambient", glm::vec3(0.3f, 1.0f, 1.0f));
	this->shaders[0].setVec3("pointlight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
	this->shaders[0].setVec3("pointlight.specular", glm::vec3(0.3f, 0.3f, 0.3f));

	this->shaders[0].setInt("material.txtr", 0);
	this->shaders[0].setFloat("material.shininess", 32.0f);
	this->shaders[0].setInt("normalTxtr", 1);

	uint rockIndex = 0;
	uint start = CHECKPOINTS * 3 + 2, end = CHECKPOINTS * 3 + 301;
	for (int i = 0; i < this->models.size(); i++) {
		Model model = this->models[i];

		// Update process
		if (i == 1 || (i % 3 == 2 && i < CHECKPOINTS * 3)) { // Spaceship and planet
			this->models[i].setRotation(glm::vec3(0.0f, float(Clock::time) * (i == 1 ? 10.0f : 90.0f), 0.0f));
			if (i != 1 && this->models[i].getTexture() != 3 && Model::dist(this->models[0].getPosition(), this->models[i].getPosition()) <= 15.0f)
			{
				Spacecraft::tasks--;
				this->models[i].setTexture(3);
			}
			glDisable(GL_CULL_FACE);
		}
		
		if (i > 1 && i % 3 == 1 && i < start) { // Food
			if (this->models[i].getAlpha() != 0.0f && Model::dist(this->models[0].getPosition(), this->models[i].getPosition()) <= 4.0f) {
				Spacecraft::tasks--;
				this->models[i].setAlpha(0.0f);
			}
		}
		
		if (i >= start && i <= end) { // Rock
			this->rocks[rockIndex].angle += float(Clock::delta) * 0.1f;
			this->models[i].setPosition(this->models[1].getPosition() + glm::vec3(this->rocks[rockIndex].radius * glm::sin(this->rocks[rockIndex].angle), 10.0f, this->rocks[rockIndex].radius * glm::cos(this->rocks[rockIndex].angle)));
			this->models[i].setRotation(this->rocks[rockIndex].rotation);
			this->models[i].setScale(glm::vec3(this->rocks[rockIndex].scale));
			rockIndex++;
		}

		this->shaders[0].setInt("useNormal", i == 1 || (i >= start && i <= end));
		this->models[0].setTexture(Spacecraft::tasks > 0 ? 0 : 1);

		// Shader uniform values
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), model.getScale());
		glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(model.getRotation().x), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(model.getRotation().y), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(model.getRotation().z), glm::vec3(0, 0, 1));
		glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), model.getPosition());

		this->shaders[0].setMat4("scaleMatrix", scaleMatrix);
		this->shaders[0].setMat4("rotationMatrix", rotationMatrix);
		this->shaders[0].setMat4("translateMatrix", translateMatrix);
		this->shaders[0].setMat4("viewMatrix", viewMatrix);
		this->shaders[0].setMat4("projectionMatrix", projectionMatrix);

		// Draw with texture
		if (this->models[i].getAlpha()) {
			this->textures[model.getTexture()].bind(0);
			if (i == 1) {
				this->textures[5].bind(1);
			} else if (i >= start && i <= end) {
				this->textures[8].bind(1);
			}
			model.draw();
			this->textures[model.getTexture()].unbind();
			if (i == 1) {
				this->textures[5].unbind();
			} else if (i >= start && i <= end) {
				this->textures[8].unbind();
			}
		}
		glEnable(GL_CULL_FACE);
	}

	// Skybox
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	this->shaders[1].use();

	this->shaders[1].setMat4("view", skyboxViewMatrix);
	this->shaders[1].setMat4("projection", projectionMatrix);

	glBindVertexArray(Skybox::vaoID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	// Unbind
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	glUseProgram(0);
}

// Create shader
void Window::createShader(const char* vertexPath, const char* fragmentPath)
{
	Shader shader(vertexPath, fragmentPath);
	this->shaders.push_back(shader);
}

// Create model
void Window::createModel(const char* objPath, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, uint txtr)
{
	Model model(objPath, pos, rot, scl, txtr);
	this->models.push_back(model);
}

// Create alien
void Window::createAlien(glm::vec3 pos, int type)
{
	Alien alien;
	alien.index = this->models.size();
	alien.position = pos;
	alien.rotation = glm::vec3(0.0f, 0.0f, 0.0f);

	this->createModel("Resources/alienvehicle.obj", alien.position, alien.rotation, glm::vec3(2.0f, 2.0f, 2.0f), 2);									// Alien vehicle (index)
	this->createModel("Resources/alienpeople2.obj", alien.position + glm::vec3(0.0f, 6.0f, 0.0f), alien.rotation, glm::vec3(2.0f, 2.0f, 2.0f), 2);		// Alien people (index + 1)
	if (type) {
		this->createModel("Resources/chicken2.obj", alien.position + glm::vec3(15.0f, 2.0f, 0.0f), alien.rotation, glm::vec3(0.02f, 0.02f, 0.02f), 7);	// Chicken (index + 2)
	} else {
		this->createModel("Resources/banana.obj", alien.position + glm::vec3(15.0f, 2.0f, 0.0f), alien.rotation, glm::vec3(0.7f, 0.7f, 0.7f), 9);		// Banana (index + 2)
	}
	
	this->aliens.push_back(alien);
}

// Create rock
void Window::createRock(int index)
{
	Rock rock;

	rock.index = this->models.size();
	rock.radius = glm::linearRand(80.0f, 110.0f);
	rock.angle = glm::radians(360.0f * index / 300.0f);
	rock.rotation = glm::vec3(glm::linearRand(0.0f, 360.0f), glm::linearRand(0.0f, 360.0f), glm::linearRand(0.0f, 360.0f));
	rock.scale = glm::linearRand(0.25f, 2.5f);

	this->createModel("Resources/rock.obj", this->models[10].getPosition() + glm::vec3(rock.radius * glm::sin(rock.angle), 10.0f, rock.radius * glm::cos(rock.angle)), rock.rotation, glm::vec3(rock.scale, rock.scale, rock.scale), 6); // Rock (index)
	this->rocks.push_back(rock);
}

// Create texture
void Window::createTexture(const char* texturePath)
{
	Texture texture(texturePath);
	this->textures.push_back(texture);
}

// Get status value
int Window::getStatus()
{
	return this->status;
}

// Frame buffer callback
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// Mouse button callback
void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	Controls::leftMouse = button == GLFW_MOUSE_BUTTON_LEFT ? action == GLFW_PRESS : Controls::leftMouse;
}

// Cursor position callback
void Window::cursor_position_callback(GLFWwindow* window, double x, double y)
{
	if (Controls::leftMouse)
	{
		float xoffset = float(x - Controls::posX);
		const float sensitivity = 0.1f;
		xoffset *= sensitivity;

		Spacecraft::rotation.y -= xoffset;
	}
	Controls::posX = x;
}

// Mouse scroll callback
void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{

}

// Keyboard key callback
void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	glm::mat4 worldMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(Spacecraft::rotation.x), glm::vec3(1, 0, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(Spacecraft::rotation.y - 180.0f), glm::vec3(0, 1, 0)) * glm::rotate(glm::mat4(1.0f), glm::radians(Spacecraft::rotation.z), glm::vec3(0, 0, 1));
	glm::vec3 front = glm::vec3(worldMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
	glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

	const float sensitivity = 2.0f;
	front *= sensitivity;
	right *= sensitivity;

	if (key == GLFW_KEY_UP && action == GLFW_REPEAT) { Spacecraft::position += front; }
	if (key == GLFW_KEY_DOWN && action == GLFW_REPEAT) { Spacecraft::position -= front; }
	if (key == GLFW_KEY_LEFT && action == GLFW_REPEAT) { Spacecraft::position -= right; }
	if (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT) { Spacecraft::position += right; }
}