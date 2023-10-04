#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
private:
  GLFWwindow *window;

public:
  Window();
  ~Window();
  void display(void (*loop)());
};
