#include "env_manager.h"

bool env::load(const char* mapFile)
{
  sf::Clock Clock;
  Clock.restart();

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(mapFile);
  //int w = doc.child("level").attribute("width").as_int();
  //int h = doc.child("level").attribute("height").as_int();
  //std::cout << "Load result: " << result.description() << ", widh : " << w << " ; height : " << h << std::endl;

  if (!m_tileset.loadFromFile("environements/" + std::string(doc.child("level").attribute("tileset").value()))) return false;

  //int nombreLayer(std::distance(doc.child("level").begin(), doc.child("level").end()));

  layerMinMax = doc.child("level").attribute("coucheTerre").as_int();
  std::cout << layerMinMax << '\n';

  for (pugi::xml_node tiles = doc.child("level").first_child(); tiles; tiles = tiles.next_sibling())
  {
    //std::cout << tiles.attribute("type").value() << '\n';
    if (tiles.attribute("type").value()==std::string("collision"))
    {
      for (pugi::xml_node tile = tiles.first_child(); tile; tile = tile.next_sibling())
      {
        //std::cout << "x = " << tile.attribute("x").as_int() << "; y = " << tile.attribute("y").as_int() << "; w = " << tile.attribute("w").as_int() << "; h = " << tile.attribute("h").as_int();
        //std::cout << std::endl;

        int x(tile.attribute("x").as_int());
        int y(tile.attribute("y").as_int());
        int rectWidth(tile.attribute("w").as_int());
        int rectHeight(tile.attribute("h").as_int());
        boitesCollision.push_back(sf::FloatRect(x, y, rectWidth, rectHeight));
      }
    }
    else if (tiles.attribute("type").value()==std::string("collisionCercle"))
    {
      for (pugi::xml_node tile = tiles.first_child(); tile; tile = tile.next_sibling())
      {
        int x(tile.attribute("x").as_int());
        int y(tile.attribute("y").as_int());
        int rectWidth(tile.attribute("w").as_int());
        int rectHeight(tile.attribute("h").as_int());
        sf::Vector2f position(x+rectWidth/2, y+rectHeight/2);
        float taille = rectHeight/2;
        cerclesCollision.push_back(FloatCircle(position, taille));
      }
    }
    else if (tiles.attribute("type").value()==std::string("start"))
    {
      pugi::xml_node tile = tiles.first_child();
      for (size_t i = 0; i < 4; i++) {
        positionsDepart[i].x = tile.attribute("x").as_int();
        positionsDepart[i].y = tile.attribute("y").as_int();
        tile = tile.next_sibling();
      }
    }
    else
    {
      couchesTuile.push_back(layer());

      couchesTuile.back().zOrder = tiles.attribute("z").as_int();
      size_t count = std::distance(tiles.begin(), tiles.end());
      //std::cout << count << std::endl;
      couchesTuile.back().tuiles.setPrimitiveType(sf::Quads);
      int vertexSize = couchesTuile.back().tuiles.getVertexCount();
      //std::cout << vertexSize << '\n';
      couchesTuile.back().tuiles.resize(/*vertexSize + */count*4);

      int tileSize = tiles.attribute("tilesize").as_int();

      int q(vertexSize);
      for (pugi::xml_node tile = tiles.first_child(); tile; tile = tile.next_sibling())
      {
        //std::cout << "x = " << tile.attribute("x").as_int() << "; y = " << tile.attribute("y").as_int() << "; tx = " << tile.attribute("tx").as_int() << "; ty = " << tile.attribute("ty").as_int();
        //std::cout << std::endl;

        int x(tile.attribute("x").as_int());
        int y(tile.attribute("y").as_int());
        int tx(tile.attribute("tx").as_int());
        int ty(tile.attribute("ty").as_int());
        couchesTuile.back().tuiles[q].position = sf::Vector2f( x*tileSize, y*tileSize );
        couchesTuile.back().tuiles[q+1].position = sf::Vector2f( (x+1)*tileSize, y*tileSize );
        couchesTuile.back().tuiles[q+2].position = sf::Vector2f( (x+1)*tileSize, (y+1)*tileSize );
        couchesTuile.back().tuiles[q+3].position = sf::Vector2f( x*tileSize, (y+1)*tileSize );

        couchesTuile.back().tuiles[q].texCoords = sf::Vector2f( tx*tileSize, ty*tileSize );
        couchesTuile.back().tuiles[q+1].texCoords = sf::Vector2f( (tx+1)*tileSize, ty*tileSize );
        couchesTuile.back().tuiles[q+2].texCoords = sf::Vector2f( (tx+1)*tileSize, (ty+1)*tileSize );
        couchesTuile.back().tuiles[q+3].texCoords = sf::Vector2f( tx*tileSize, (ty+1)*tileSize );

        q+=4;
      }
    }
  }

  std::sort (couchesTuile.begin(), couchesTuile.end()); // ordonner les layers en fonction de leur "z" grâce à la surcharge operateur "<" de layer

  std::cout << Clock.restart().asSeconds() << '\n';

  std::cout << couchesTuile.size() << '\n';

  /*for (size_t c = 0; c < cerclesCollision.size(); c++) {
    std::cout << "x : " << cerclesCollision[c].position.x << " y : " << cerclesCollision[c].position.y << " taille : " << cerclesCollision[c].taille << '\n';
  }

  for (size_t rect = 0; rect < boitesCollision.size(); rect++) {
    std::cout << "x : " << boitesCollision[rect].left << " y : " << boitesCollision[rect].top << " w : " << boitesCollision[rect].width << " h : " << boitesCollision[rect].height << '\n';
  }

  for (size_t rect = 0; rect < 4; rect++) {
    std::cout << "x : " << positionsDepart[rect].x << " y : " << positionsDepart[rect].y << '\n';
  }*/

  dessinAvant = true;
  return true;
}

bool env::collide(const sf::FloatRect &rectangleATester)
{
  for (size_t rect = 0; rect < boitesCollision.size(); rect++) {
    if (boitesCollision[rect].intersects(rectangleATester)) return true;
  }
  for (size_t cercle = 0; cercle < cerclesCollision.size(); cercle++) {
    int plusPresX = clamp(cerclesCollision[cercle].position.x, rectangleATester.left, rectangleATester.left + rectangleATester.width);
    int plusPresY = clamp(cerclesCollision[cercle].position.y, rectangleATester.top, rectangleATester.top + rectangleATester.height);
    if ((plusPresX-cerclesCollision[cercle].position.x)*(plusPresX-cerclesCollision[cercle].position.x)+(plusPresY-cerclesCollision[cercle].position.y)*(plusPresY-cerclesCollision[cercle].position.y)<cerclesCollision[cercle].taille*cerclesCollision[cercle].taille) return true;
  }
  return false;
}

sf::Vector2f env::getPositionDepart(int persoNumero)
{
  if (persoNumero>=0&&persoNumero<4) return positionsDepart[persoNumero];
  else return sf::Vector2f(0,0);
}

void env::dessinAv()
{
  dessinAvant = !dessinAvant;
}

void env::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
  states.transform *= getTransform();

  states.texture = &m_tileset;

  /*//méthode 1 : un seul tableau (fusion des differentes couches)
  //sf::Clock Clock;
  //Clock.restart();

  sf::VertexArray tuilesADessiner(sf::Quads);

  for (size_t j = 0; j < couchesTuile.size(); j++) {
    tuilesADessiner.resize(tuilesADessiner.getVertexCount()+couchesTuile[j].tuiles.getVertexCount());
    for (size_t k = 0; k < couchesTuile[j].tuiles.getVertexCount(); k++) {
      tuilesADessiner.append(couchesTuile[j].tuiles[k]);
    }
  }

  //std::cout << Clock.restart().asSeconds() << '\n';

  target.draw(tuilesADessiner, states);
  //fin méthode 1*/

  //méthode 2 : on dessine les couches séparément
  for (size_t i = 0; i < couchesTuile.size(); i++) {
    if (couchesTuile[i].zOrder<=layerMinMax && dessinAvant == true)
    {
      target.draw(couchesTuile[i].tuiles, states);
    }
    else if (couchesTuile[i].zOrder>layerMinMax && dessinAvant == false)
    {
      target.draw(couchesTuile[i].tuiles, states);
    }
  }

  //fin méthode 2
}
