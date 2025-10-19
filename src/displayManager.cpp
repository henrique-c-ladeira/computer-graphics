#include "displayManager.h"

DisplayManager::DisplayManager() {}
DisplayManager::~DisplayManager() {}

bool DisplayManager::init()
{
}

Window *Window::startup()
{
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  Window *window = new Window();
  window->handle = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);
  if (!window->handle)
  {
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window->handle);
  return window;
};

void DisplayManager::shutdown()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void DisplayManager::run(void (*loop)())
{
  while (!glfwWindowShouldClose(handle))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    loop();

    glFlush();
    /* Swap front and back buffers */
    glfwSwapBuffers(handle);

    /* Poll for and process events */
    glfwPollEvents();
  }
};
