#ifndef OPPONENT_HH
#define OPPONENT_HH

#include <SFML/Graphics.hpp>
#include "Ball.hh"

sf::RectangleShape createOpponent(sf::RenderWindow& window);

class Opponent
{
public:
  Opponent(sf::RenderWindow& window);

  sf::RectangleShape opponent;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;
  sf::Vector2f currentPosition;

  void setVelocity(Ball& ball, sf::RenderWindow& window);
  void handleWallCollision(sf::RenderWindow& window);

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window);
  void move(sf::Time timeStep);
  void reset(sf::RenderWindow& window);
};

#endif