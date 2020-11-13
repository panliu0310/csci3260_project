#pragma once
#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>

class Window
{
  // Constructor and destructor
  public:
    Window();

  // Variables
  private:
    GLFWwindow* window{};
    int status = 0;
    const int width = 800;
    const int height = 600;

  // Functions
  public:
    void get_OpenGL_info();
    void sendDataToOpenGL();
    void initializedGL(void);
    void paintGL(void);

  // Getters
  public:
    int getStatus();

  // Callbacks
  public:
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double x, double y);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
};