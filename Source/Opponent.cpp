#include "Opponent.hh"

sf::RectangleShape createOpponentShape(const sf::RenderWindow& window)
{
  sf::RectangleShape shape{sf::Vector2f{10.f, 100.f}};
  shape.setPosition(window.getSize().x - (10 + shape.getGlobalBounds().width),  window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  shape.setFillColor(sf::Color::Blue);
  return shape;
}

Opponent::Opponent(const sf::RenderWindow& window)
  : shape{createOpponentShape(window)}
  , velocity{0.f, 0.f}
  , previousPosition{shape.getPosition()}
{
}

sf::FloatRect Opponent::getGlobalAABB() const
{
  return shape.getGlobalBounds();
}


void Opponent::setVelocity(const Ball& ball, const sf::RenderWindow& window)
{
  if (ball.getGlobalAABB().left > window.getSize().x / 2.f && ball.getVelocity().x > 0)
  {
    if (ball.getPosition().y < shape.getGlobalBounds().top + shape.getGlobalBounds().height / 2)
    {
      velocity.y = -300.f;
    }
    else if (ball.getPosition().y > shape.getGlobalBounds().top + shape.getGlobalBounds().height / 2)
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

void Opponent::handleWallCollision(const sf::RenderWindow& window)
{
  if (shape.getGlobalBounds().top < 0)
  {
    shape.setPosition(shape.getPosition().x, 0);
  }
  else if (shape.getGlobalBounds().top + shape.getGlobalBounds().height > window.getSize().y)
  {
    shape.setPosition(shape.getPosition().x, window.getSize().y - shape.getGlobalBounds().height);
  }
}

void Opponent::draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window)
{
  const float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  const sf::Vector2f currentPosition{shape.getPosition()};
  const sf::Vector2f interpolatedOpponentPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  shape.setPosition(interpolatedOpponentPosition);
  window.draw(shape);
  shape.setPosition(currentPosition);
}

void Opponent::move(const sf::Time& timeStep)
{
  previousPosition = shape.getPosition();
  shape.move(velocity * timeStep.asSeconds());
}

void Opponent::reset(const sf::RenderWindow& window)
{
  shape.setPosition(window.getSize().x - (10 + shape.getGlobalBounds().width),  window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  velocity.x = 0.f;
  velocity.y = 0.f;
}