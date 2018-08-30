#ifndef BALL_HH
#define BALL_HH

#include <SFML/Graphics.hpp>

sf::CircleShape createBallShape(sf::RenderWindow& window);

class Ball
{
public:
  Ball(sf::RenderWindow& window);

  sf::CircleShape ball;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;
  sf::Vector2f currentPosition;

  bool isPenetratingIntoLeftWall(sf::RenderWindow& window);
  bool isPenetratingIntoRightWall(sf::RenderWindow& window);
  bool isPenetratingIntoTopWall(sf::RenderWindow& window);
  bool isPenetratingIntoBottomWall(sf::RenderWindow& window);
  void handleWallCollision(sf::RenderWindow& window);

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window);
  void move(sf::Time timeStep);
  void reset(sf::RenderWindow& window);
};

#endif