#ifndef OPPONENT_HH
#define OPPONENT_HH

#include <SFML/Graphics.hpp>
#include "Ball.hh"

sf::RectangleShape createOpponentShape(sf::RenderWindow& window);

class Opponent
{
public:
  Opponent(sf::RenderWindow& window);

  sf::RectangleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;

  void setVelocity(Ball& ball, sf::RenderWindow& window);
  void handleWallCollision(sf::RenderWindow& window);

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window);
  void move(sf::Time timeStep);
  void reset(sf::RenderWindow& window);
};

#endif