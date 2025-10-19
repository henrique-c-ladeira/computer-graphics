#pragma once
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
private:
  GLFWwindow *handle;

public:
  Window();
  ~Window();
  static Window *startup();
  void display(void (*loop)());
};
