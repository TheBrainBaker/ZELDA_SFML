#include "animable.h"

animable::animable():animationDuSprite(0), frameBoucle(0){}

bool animable::load(const char* animFile)
{
  sf::Clock Clock;
  Clock.restart();

  pugi::xml_document doc;
  pugi::xml_parse_result result = doc.load_file(animFile);
  //std::cout << "Load result: " << result.description() << std::endl;

  if (!spriteSheet.loadFromFile("ressources/" + std::string(doc.child("img").attribute("name").value()))) return false;
  sf::Sprite::setTexture(spriteSheet);

  for (pugi::xml_node anim = doc.child("img").first_child().first_child().first_child(); anim; anim = anim.next_sibling())
  {
    std::cout << anim.attribute("name").value() << '\n'; // le nom de l'animation

    animations.push_back(animation());
    animations.back().vitesse = 6;

    pugi::xml_node hitBoxNode = anim.child("hitBox");
    if (hitBoxNode)
    {
      std::cout << "ddd" << '\n';
      float x(hitBoxNode.attribute("x").as_float());
      float y(hitBoxNode.attribute("y").as_float());
      float rectWidth(hitBoxNode.attribute("w").as_float());
      float rectHeight(hitBoxNode.attribute("h").as_float());
      std::cout << x << '\n';
      animations.back().hitBox = sf::FloatRect(x, y, rectWidth, rectHeight);
    }
    else
    {
      hitBoxNode = anim.first_child();
      float x(0);
      float y(0);
      float rectWidth(hitBoxNode.attribute("w").as_float());
      float rectHeight(hitBoxNode.attribute("h").as_float());
      rectHeight /= 2;
      y+=rectHeight;
      std::cout << x << '\n';
      animations.back().hitBox = sf::FloatRect(x, y, rectWidth, rectHeight);
    }
    for (pugi::xml_node frame = anim.first_child(); frame; frame = frame.next_sibling())
    {
      //std::cout << "x = " << frame.attribute("x").as_int() << "; y = " << frame.attribute("y").as_int() << "; w = " << frame.attribute("w").as_int() << "; h = " << frame.attribute("h").as_int() << std::endl;

      int x(frame.attribute("x").as_int());
      int y(frame.attribute("y").as_int());
      int rectWidth(frame.attribute("w").as_int());
      int rectHeight(frame.attribute("h").as_int());
      int ox(frame.attribute("ox").as_float(-1));
      int oy(frame.attribute("oy").as_float(-1));
      if (ox==-1) ox=rectWidth/2;
      if (oy==-1) oy=rectHeight/2;
      animations.back().animationRects.push_back(sf::IntRect(x, y, rectWidth, rectHeight));
      animations.back().origins.push_back(sf::Vector2f(ox, oy));
    }
    /*
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
      couchesTuile.back().tuiles.resize(vertexSize + count*4);

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
  */}

  std::cout << Clock.restart().asSeconds() << '\n';

  /*for (size_t anim = 0; anim < animationsRect.size(); anim++) {
    for (size_t frame = 0; frame < animationsRect[anim].size(); frame++) {
      std::cout << "x : " << animationsRect[anim][frame].left << " y : " << animationsRect[anim][frame].top << " w : " << animationsRect[anim][frame].width << " h : " << animationsRect[anim][frame].height << '\n';
    }
  }*/

  animationDuSprite = &animations[0];
  this->update();

  return true;
}

void animable::update()
{
  sf::Sprite::setOrigin(animationDuSprite->origins[animationDuSprite->frame]);
  sf::Sprite::setTextureRect(animationDuSprite->animationRects[animationDuSprite->frame]);
  //std::cout << animationDuSprite->frame << '\n';
  if (frameBoucle>0) frameBoucle--;
  else
  {
    animationDuSprite->frame++;
    if (animationDuSprite->frame>=animationDuSprite->animationRects.size()) animationDuSprite->frame=0;
    frameBoucle=animationDuSprite->vitesse;
  }
}

void animable::changerAnimation(int animationNumero, bool miroir)
{
  animationDuSprite = &animations[animationNumero];
  sf::Sprite::setTextureRect(animationDuSprite->animationRects[0]);
  //std::cout << frameBoucle << '\n';
  if (miroir) sf::Sprite::setScale(-1,1);
  else sf::Sprite::setScale(1,1);
}

sf::FloatRect animable::getHitBox()
{
  sf::FloatRect tHitBox(animationDuSprite->hitBox);
  tHitBox.left=sf::Sprite::getPosition().x-tHitBox.width/2;
  tHitBox.top=sf::Sprite::getPosition().y;
  return tHitBox;
}
