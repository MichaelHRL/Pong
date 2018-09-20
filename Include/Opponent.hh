#ifndef OPPONENT_HH
#define OPPONENT_HH

#include <SFML/Graphics.hpp>
#include "Ball.hh"

sf::RectangleShape createOpponentShape(const sf::RenderWindow& window);

class Opponent
{
public:
  Opponent(const sf::RenderWindow& window);

  sf::FloatRect getGlobalAABB() const;

  void setVelocity(const Ball& ball, const sf::RenderWindow& window);
  void handleWallCollision(const sf::RenderWindow& window);

  void draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window);
  void move(const sf::Time& timeStep);
  void reset(const sf::RenderWindow& window);
private:
  sf::RectangleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;
};

#endif