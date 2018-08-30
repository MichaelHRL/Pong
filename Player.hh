#ifndef PLAYER_HH
#define PLAYER_HH

#include <SFML/Graphics.hpp>

sf::RectangleShape createPlayerShape(sf::RenderWindow& window);

class Player
{
public:
  Player(sf::RenderWindow& window);

  sf::RectangleShape shape;
  sf::Vector2f velocity;
  bool movePlayerUp;
  bool movePlayerDown;
  sf::Vector2f previousPosition;

  void setVelocity();

  bool isPenetratingTopWall(sf::RenderWindow& window);
  bool isPenetratingBottomWall(sf::RenderWindow& window);
  void handleWallCollision(sf::RenderWindow& window);

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window);
  void move(sf::Time timeStep);
  void reset(sf::RenderWindow& window);
};

#endif