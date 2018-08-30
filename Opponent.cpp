#include "Opponent.hh"

sf::RectangleShape createOpponentShape(sf::RenderWindow& window)
{
  sf::RectangleShape shape{sf::Vector2f{10.f, 100.f}};
  shape.setPosition(window.getSize().x - (10 + shape.getGlobalBounds().width),  window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  shape.setFillColor(sf::Color::Blue);
  return shape;
}

Opponent::Opponent(sf::RenderWindow& window)
  : shape{createOpponentShape(window)}
  , velocity{0.f, 0.f}
  , previousPosition{shape.getPosition()}
  , currentPosition{shape.getPosition()}
{
}

void Opponent::setVelocity(Ball& ball, sf::RenderWindow& window)
{
  if (ball.shape.getGlobalBounds().left > window.getSize().x / 2.f && ball.velocity.x > 0)
  {
    if (ball.shape.getPosition().y < shape.getGlobalBounds().top + shape.getGlobalBounds().height / 2)
    {
      velocity.y = -300.f;
    }
    else if (ball.shape.getPosition().y > shape.getGlobalBounds().top + shape.getGlobalBounds().height / 2)
    {
      velocity.y = 300.f;
    }
    else
    {
      velocity.y = 0.f;
    }
  }
  else
  {
    velocity.y = 0.f;
  }
}

void Opponent::handleWallCollision(sf::RenderWindow& window)
{
  if (shape.getGlobalBounds().top < 0)
  {
    shape.setPosition(shape.getPosition().x, 0);
    currentPosition = shape.getPosition();
  }
  else if (shape.getGlobalBounds().top + shape.getGlobalBounds().height > window.getSize().y)
  {
    shape.setPosition(shape.getPosition().x, window.getSize().y - shape.getGlobalBounds().height);
    currentPosition = shape.getPosition();
  }
}

void Opponent::draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedOpponentPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  shape.setPosition(interpolatedOpponentPosition);
  window.draw(shape);
  shape.setPosition(currentPosition);
}

void Opponent::move(sf::Time timeStep)
{
  previousPosition = currentPosition;
  shape.move(velocity * timeStep.asSeconds());
  currentPosition = shape.getPosition();
}

void Opponent::reset(sf::RenderWindow& window)
{
  shape.setPosition(window.getSize().x - (10 + shape.getGlobalBounds().width),  window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  velocity.x = 0.f;
  velocity.y = 0.f;
}