#ifndef BALL_HH
#define BALL_HH

#include <SFML/Graphics.hpp>

class Ball
{
public:
  Ball(const sf::FloatRect& window);

  sf::Vector2f getPosition() const;
  sf::Vector2f getVelocity() const;

  void setVelocity(float x, float y);

  sf::FloatRect getGlobalAABB() const;

  bool isPenetratingIntoLeftWall() const;
  bool isPenetratingIntoRightWall() const;
  bool isPenetratingIntoTopWall() const;
  bool isPenetratingIntoBottomWall() const;
  void handleWallCollision();

  void draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window);
  void move(const sf::Time& timeStep);
  void reset();
private:
  sf::Vector2f startPosition;
  sf::Vector2f startVelocity;

  sf::CircleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;

  const sf::FloatRect& playingField;
};

#endif