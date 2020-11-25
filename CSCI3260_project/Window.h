#pragma once
#include "include.h"
#include "Model.h"
#include "Shader.h"

typedef unsigned int uint;

struct Alien
{
  uint index;
  glm::vec3 position, rotation;
};

class Window
{
  // Constructor
  public:
    Window();

  // Variables
  private:
    GLFWwindow* window{};
    int status = 0;
    const int width = 800, height = 600;
    std::vector<Shader> shaders;
    std::vector<Model> models;
    std::vector<Alien> aliens;
    std::vector<Texture> textures;

  // Functions
  public:
    void get_OpenGL_info();
    void sendDataToOpenGL();
    void initializedGL(void);
    void paintGL(void);

  public:
    void createShader(const char* vertexPath, const char* fragmentPath);
    void removeShader(uint index);

    void createModel(const char* objPath, glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, unsigned int txtr);
    void removeModel(uint index);

    void createAlien(glm::vec3 position);
    void removeAlien(uint index);

    void createTexture(const char* texturePath);
    void removeTexture(uint index);
    

  // Getters
  public:
    int getStatus();
    int getModelSize();

    Shader getShader(uint index);
    Model getModel(uint index);
    Alien getAlien(uint index);
    Texture getTexture(uint index);

  // Callbacks
  public:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};