#include "Ball.hh"

sf::CircleShape createBallShape(sf::RenderWindow& window)
{
  sf::CircleShape ball{5.f};
  ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  ball.setFillColor(sf::Color::Green);
  return ball;
}

Ball::Ball(sf::RenderWindow& window)
  : ball{createBallShape(window)}
  , velocity{424.f, 0.f}
  , previousPosition{ball.getPosition()}
  , currentPosition{ball.getPosition()}
{
}

bool Ball::isPenetratingIntoLeftWall(sf::RenderWindow& window)
{
  // The check for velocity is neccessary because it ensures that the ball doesn't stay trapped within a surface
  return ball.getGlobalBounds().left < 0 && velocity.x < 0;
}

bool Ball::isPenetratingIntoRightWall(sf::RenderWindow& window)
{
  return ball.getGlobalBounds().left + ball.getGlobalBounds().width > window.getSize().x && velocity.x > 0;
}

bool Ball::isPenetratingIntoTopWall(sf::RenderWindow& window)
{
  return ball.getGlobalBounds().top < 0 && velocity.y < 0;
}

bool Ball::isPenetratingIntoBottomWall(sf::RenderWindow& window)
{
  return ball.getGlobalBounds().top + ball.getGlobalBounds().height > window.getSize().y && velocity.y > 0;
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
  ball.setPosition(interpolatedBallPosition);
  window.draw(ball);
  ball.setPosition(currentPosition);
}

void Ball::move(sf::Time timeStep)
{
  previousPosition = currentPosition;
  ball.move(velocity * timeStep.asSeconds());
  currentPosition = ball.getPosition();
}

void Ball::reset(sf::RenderWindow& window)
{
  ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  velocity.x = 424.f;
  velocity.y = 0.f;
}