#ifndef DEF_Game
#define DEF_Game

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include "env_manager.h"
#include "entite.h"

class Game{
public:
  Game();

  void HandleInput();
  void Update();
  void Render();
  bool IsDone();

private:
  sf::RenderWindow m_window;
  sf::Clock m_clock;
  sf::Time m_elapsed;
  float frametime;
  e c;
  env environement;
};

#endif
