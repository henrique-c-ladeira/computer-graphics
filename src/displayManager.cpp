#include "displayManager.h"

DisplayManager::DisplayManager() {}
DisplayManager::~DisplayManager() {}

bool DisplayManager::init()
{
  if (!glfwInit())
    return false;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

  window = glfwCreateWindow(512, 512, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return false;
  }
  glfwMakeContextCurrent(window);
  return true;
};

void DisplayManager::shutdown()
{
  glfwDestroyWindow(window);
  glfwTerminate();
}

void DisplayManager::run(void (*loop)())
{
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);
    loop();

    glFlush();
    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }
};
