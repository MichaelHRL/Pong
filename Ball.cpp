#include "Ball.hh"

sf::CircleShape createBallShape(sf::RenderWindow& window)
{
  sf::CircleShape shape{5.f};
  shape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  shape.setFillColor(sf::Color::Green);
  return shape;
}

Ball::Ball(sf::RenderWindow& window)
  : shape{createBallShape(window)}
  , velocity{424.f, 0.f}
  , previousPosition{shape.getPosition()}
  , currentPosition{shape.getPosition()}
{
}

bool Ball::isPenetratingIntoLeftWall(sf::RenderWindow& window)
{
  // The check for velocity is neccessary because it ensures that the shape doesn't stay trapped within a surface
  return shape.getGlobalBounds().left < 0 && velocity.x < 0;
}

bool Ball::isPenetratingIntoRightWall(sf::RenderWindow& window)
{
  return shape.getGlobalBounds().left + shape.getGlobalBounds().width > window.getSize().x && velocity.x > 0;
}

bool Ball::isPenetratingIntoTopWall(sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top < 0 && velocity.y < 0;
}

bool Ball::isPenetratingIntoBottomWall(sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top + shape.getGlobalBounds().height > window.getSize().y && velocity.y > 0;
}

void Ball::handleWallCollision(sf::RenderWindow& window)
{
  if (isPenetratingIntoLeftWall(window) || isPenetratingIntoRightWall(window))
  {
    velocity.x *= -1;
  }
  if (isPenetratingIntoTopWall(window) || isPenetratingIntoBottomWall(window))
  {
    velocity.y *= -1;
  }
}

void Ball::draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedBallPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  shape.setPosition(interpolatedBallPosition);
  window.draw(shape);
  shape.setPosition(currentPosition);
}

void Ball::move(sf::Time timeStep)
{
  previousPosition = currentPosition;
  shape.move(velocity * timeStep.asSeconds());
  currentPosition = shape.getPosition();
}

void Ball::reset(sf::RenderWindow& window)
{
  shape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  velocity.x = 424.f;
  velocity.y = 0.f;
}