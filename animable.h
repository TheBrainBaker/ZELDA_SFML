#ifndef DEF_ANIMABLE
#define DEF_ANIMABLE

#include <SFML/Graphics.hpp>
#include "pugixml.hpp"
#include <iostream>
#include <iterator>
#include <string>

struct animation {
  std::vector<sf::IntRect> animationRects;
  int frame;
  int vitesse;
  std::vector<sf::Vector2f> origins;
  sf::FloatRect hitBox;
};

class animable : public sf::Sprite
{
public:
  animable();
  bool load(const char* animFile);
  void update();
  void changerAnimation(int animationNumero, bool miroir);
  sf::FloatRect getHitBox();

private:
  sf::Texture spriteSheet;
  std::vector<animation> animations;
  animation* animationDuSprite;
  int frameBoucle; //pour la vitesse des animations
};

#endif
