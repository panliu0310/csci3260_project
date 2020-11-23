#pragma once
#include "Model.h"
#include "Shader.h"

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
    std::vector<Texture> textures;

  // Functions
  public:
    void get_OpenGL_info();
    void sendDataToOpenGL();
    void initializedGL(void);
    void paintGL(void);

  public:
    void createShader(const char* vertexPath, const char* fragmentPath);
    void removeShader(unsigned int index);
    void createModel(const char* objPath);
    void removeModel(unsigned int index);
    void createTexture(const char* texturePath);
    void removeTexture(unsigned int index);

  // Getters
  public:
    int getStatus();
    Shader getShader(unsigned int index);
    Model getModel(unsigned int index);
    Texture getTexture(unsigned int index);

  // Callbacks
  public:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};