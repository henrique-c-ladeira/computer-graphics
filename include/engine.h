#pragma once
#include "displayManager.h"
#include "Window.h"

class Engine
{
public:
  Engine();
  ~Engine();

  bool startup();
  void run();
  bool shutdown();

private:
  Window displayManager;
};