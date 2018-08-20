#include "Opponent.hh"

sf::RectangleShape createOpponent(sf::RenderWindow& window)
{
  sf::RectangleShape opponent{sf::Vector2f{10.f, 100.f}};
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
  opponent.setFillColor(sf::Color::Blue);
  return opponent;
}

Opponent::Opponent(sf::RenderWindow& window)
  : opponent{createOpponent(window)}
  , velocity{0.f, 0.f}
  , previousPosition{opponent.getPosition()}
  , currentPosition{opponent.getPosition()}
{
}

void Opponent::setVelocity(Ball& ball, sf::RenderWindow& window)
{
  if (ball.ball.getGlobalBounds().left > window.getSize().x / 2.f && ball.velocity.x > 0)
  {
    if (ball.ball.getPosition().y < opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
    {
      velocity.y = -300.f;
    }
    else if (ball.ball.getPosition().y > opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
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
  if (opponent.getGlobalBounds().top < 0)
  {
    opponent.setPosition(opponent.getPosition().x, 0);
    currentPosition = opponent.getPosition();
  }
  else if (opponent.getGlobalBounds().top + opponent.getGlobalBounds().height > window.getSize().y)
  {
    opponent.setPosition(opponent.getPosition().x, window.getSize().y - opponent.getGlobalBounds().height);
    currentPosition = opponent.getPosition();
  }
}

void Opponent::draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedOpponentPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  opponent.setPosition(interpolatedOpponentPosition);
  window.draw(opponent);
  opponent.setPosition(currentPosition);
}

void Opponent::move(sf::Time timeStep)
{
  previousPosition = currentPosition;
  opponent.move(velocity * timeStep.asSeconds());
  currentPosition = opponent.getPosition();
}

void Opponent::reset(sf::RenderWindow& window)
{
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
  velocity.x = 0.f;
  velocity.y = 0.f;
}