#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>

sf::RectangleShape createPlayer(sf::RenderWindow& window);

class Player
{
public:
  Player(sf::RenderWindow& window);

  sf::RectangleShape player;
  sf::Vector2f velocity;
  bool movePlayerUp;
  bool movePlayerDown;
  sf::Vector2f previousPosition;
  sf::Vector2f currentPosition;

  void setVelocity();

  bool isPenetratingTopWall(sf::RenderWindow& window);
  bool isPenetratingBottomWall(sf::RenderWindow& window);
  void handleWallCollision(sf::RenderWindow& window);

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window);
  void move(sf::Time timeStep);
  void reset(sf::RenderWindow& window);
};

#endif