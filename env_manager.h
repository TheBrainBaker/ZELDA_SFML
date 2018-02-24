#ifndef DEF_env
#define DEF_env

#include <SFML/Graphics.hpp>
#include "pugixml.hpp"
#include <iostream>
#include <iterator>
#include <string>
#include <algorithm>

struct layer {
  sf::VertexArray tuiles;
  int zOrder;

  bool operator < (const layer& B) const
  {
    return (zOrder < B.zOrder);
  }
};

struct FloatCircle {
  FloatCircle(sf::Vector2f pos, float t):position(pos),taille(t){}
  sf::Vector2f position;
  float taille;
};

class env : public sf::Drawable, public sf::Transformable
{
public:
  bool load(const char* mapFile);
  bool collide(const sf::FloatRect &rectangleATester);
  sf::Vector2f getPositionDepart(int persoNumero);
  void dessinAv();

  static float clamp(float n, float lower, float upper) {
    return std::max(lower, std::min(n, upper));
  }

private:
  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
  sf::Texture m_tileset;
  std::vector<layer> couchesTuile;
  std::vector<sf::FloatRect> boitesCollision;
  std::vector<FloatCircle> cerclesCollision;
  sf::Vector2f positionsDepart[4];
  int layerMinMax;
  bool dessinAvant;
};

#endif
