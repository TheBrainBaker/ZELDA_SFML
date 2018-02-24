#include "game.h"

Game::Game(): m_window(sf::VideoMode(800, 600), "My window"), c("ressources/newSpriteSheet.sprites"), frametime(1.0f / 120.0f){
  environement.load("environements/env3.env");
  c.setPosition(environement.getPositionDepart(0));
}

void Game::HandleInput(){

}

void Game::Update(){
  sf::Event event;
  while(m_window.pollEvent(event)){
    if(event.type == sf::Event::Closed){
      m_window.close();
    }
  }

  if(m_elapsed.asSeconds() >= frametime){
    // calculs 1/frametime fois
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !environement.collide(sf::FloatRect(c.getHitBox().left+1, c.getHitBox().top, c.getHitBox().width, c.getHitBox().height)))
    {
      c.move(1, 0);
      c.changerAnimation(1,true);
      c.update();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !environement.collide(sf::FloatRect(c.getHitBox().left-1, c.getHitBox().top, c.getHitBox().width, c.getHitBox().height)))
    {
      c.move(-1, 0);
      c.changerAnimation(1,false);
      c.update();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !environement.collide(sf::FloatRect(c.getHitBox().left, c.getHitBox().top-1, c.getHitBox().width, c.getHitBox().height)))
    {
      c.move(0, -1);
      c.changerAnimation(2,false);
      c.update();
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !environement.collide(sf::FloatRect(c.getHitBox().left, c.getHitBox().top+1, c.getHitBox().width, c.getHitBox().height)))
    {
      c.move(0, 1);
      c.changerAnimation(0,false);
      c.update();
    }

    //if (environement.collide(sf::FloatRect(sf::Vector2f(c.getPosition().x,c.getPosition().y), sf::Vector2f(18,25)))) std::cout << "collide!!!" << c.getPosition().x << c.getPosition().y << '\n';
    // fin calculs
    m_elapsed -= sf::seconds(frametime);
  }
}

void Game::Render(){
  if(m_elapsed.asSeconds() <= frametime){
    m_window.clear(sf::Color::Blue);
    m_window.draw(environement);
    environement.dessinAv();
    m_window.draw(c);
    /*sf::RectangleShape rec(sf::Vector2f(c.getHitBox().width, c.getHitBox().height));
    rec.setPosition(sf::Vector2f(c.getHitBox().left, c.getHitBox().top));
    rec.setFillColor(sf::Color::Blue);
    m_window.draw(rec);*/
    m_window.draw(environement);
    environement.dessinAv();
    m_window.display();
  }
  //std::cout << 1/m_clock.getElapsedTime().asSeconds() << '\n';
  m_elapsed += m_clock.restart();
  /*std::ostringstream ss;
  ss << m_elapsed.asSeconds();
  m_window.setTitle(ss.str());*/
}

bool Game::IsDone(){
  if (m_window.isOpen()){
    return false;
  }
  return true;
}
