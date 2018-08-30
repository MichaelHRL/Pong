#ifndef PLAYER_HH
#define PLAYER_HH

#include <SFML/Graphics.hpp>

sf::RectangleShape createPlayerShape(const sf::RenderWindow& window);

class Player
{
public:
  Player(const sf::RenderWindow& window);

  sf::RectangleShape shape;
  sf::Vector2f velocity;
  bool movePlayerUp;
  bool movePlayerDown;
  sf::Vector2f previousPosition;

  void setVelocity();

  bool isPenetratingTopWall(const sf::RenderWindow& window);
  bool isPenetratingBottomWall(const sf::RenderWindow& window);
  void handleWallCollision(const sf::RenderWindow& window);

  void draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window);
  void move(const sf::Time& timeStep);
  void reset(const sf::RenderWindow& window);
};

#endif