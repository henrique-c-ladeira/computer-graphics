#pragma once
#include "displayManager.h"

class Engine
{
public:
  Engine();
  ~Engine();

  bool init();
  void run();
  bool shutdown();

private:
  DisplayManager displayManager;
};