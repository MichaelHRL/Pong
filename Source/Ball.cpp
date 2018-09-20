#include "Ball.hh"

sf::CircleShape createBallShape(const sf::RenderWindow& window)
{
  sf::CircleShape shape{5.f};
  shape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  shape.setFillColor(sf::Color::Green);
  return shape;
}

Ball::Ball(const sf::RenderWindow& window)
  : shape{createBallShape(window)}
  , velocity{424.f, 0.f}
  , previousPosition{shape.getPosition()}
{
}

sf::Vector2f Ball::getPosition() const
{
  return shape.getPosition();
}

sf::Vector2f Ball::getVelocity() const
{
  return velocity;
}

void Ball::setVelocity(float x, float y)
{
  velocity.x = x;
  velocity.y = y;
}

sf::FloatRect Ball::getGlobalAABB() const
{
  return shape.getGlobalBounds();
}

bool Ball::isPenetratingIntoLeftWall(const sf::RenderWindow& window)
{
  // The check for velocity is neccessary because it ensures that the shape doesn't stay trapped within a surface
  return shape.getGlobalBounds().left < 0 && velocity.x < 0;
}

bool Ball::isPenetratingIntoRightWall(const sf::RenderWindow& window)
{
  return shape.getGlobalBounds().left + shape.getGlobalBounds().width > window.getSize().x && velocity.x > 0;
}

bool Ball::isPenetratingIntoTopWall(const sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top < 0 && velocity.y < 0;
}

bool Ball::isPenetratingIntoBottomWall(const sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top + shape.getGlobalBounds().height > window.getSize().y && velocity.y > 0;
}

void Ball::handleWallCollision(const sf::RenderWindow& window)
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

void Ball::draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window)
{
  const float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  const sf::Vector2f currentPosition{shape.getPosition()};
  const sf::Vector2f interpolatedBallPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  shape.setPosition(interpolatedBallPosition);
  window.draw(shape);
  shape.setPosition(currentPosition);
}

void Ball::move(const sf::Time& timeStep)
{
  previousPosition = shape.getPosition();
  shape.move(velocity * timeStep.asSeconds());
}

void Ball::reset(const sf::RenderWindow& window)
{
  shape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  velocity.x = 424.f;
  velocity.y = 0.f;
}