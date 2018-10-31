#include "Ball.hh"

Ball::Ball(const sf::FloatRect& playingField)
  : playingField{playingField}
{
  startPosition = sf::Vector2f{playingField.width / 2.f, playingField.height / 2.f};
  startVelocity = sf::Vector2f{424.f, 0.f};
  
  shape.setRadius(5.f);
  shape.setFillColor(sf::Color::Green);
  reset();

  if (!buffer.loadFromFile("../Resources/Impact.wav"))
  {
    throw "Could not load audio";
  }

  sound.setBuffer(buffer);
}

void Ball::playAudio()
{
  sound.play();
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

bool Ball::isPenetratingIntoLeftWall() const
{
  // The check for velocity is neccessary because it ensures that the ball doesn't stay trapped the playingField's area.
  return (shape.getGlobalBounds().left < playingField.left) && velocity.x < 0;
}

bool Ball::isPenetratingIntoRightWall() const
{
  return (shape.getGlobalBounds().left + shape.getGlobalBounds().width > playingField.left + playingField.width) && velocity.x > 0;
}

bool Ball::isPenetratingIntoTopWall() const
{
  return (shape.getGlobalBounds().top < playingField.top) && velocity.y < 0;
}

bool Ball::isPenetratingIntoBottomWall() const
{
  return (shape.getGlobalBounds().top + shape.getGlobalBounds().height > playingField.top + playingField.height) && velocity.y > 0;
}

void Ball::handleWallCollision()
{
  if (isPenetratingIntoLeftWall() || isPenetratingIntoRightWall())
  {
    velocity.x *= -1;
    sound.play();
  }
  if (isPenetratingIntoTopWall() || isPenetratingIntoBottomWall())
  {
    velocity.y *= -1;
    sound.play();
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

void Ball::reset()
{
  shape.setPosition(startPosition);

  velocity = startVelocity;
  previousPosition = startPosition;
}