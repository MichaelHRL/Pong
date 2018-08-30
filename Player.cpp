#include "Player.hh"

sf::RectangleShape createPlayerShape(sf::RenderWindow& window)
{
  sf::RectangleShape shape{sf::Vector2f{10.f, 100.f}};
  shape.setPosition(10, window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  shape.setFillColor(sf::Color::Red);
  return shape;
}


Player::Player(sf::RenderWindow& window)
  : shape{createPlayerShape(window)}
  , velocity{0.f, 0.f}
  , movePlayerUp{false}
  , movePlayerDown{false}
  , previousPosition{shape.getPosition()}
{
}

void Player::setVelocity()
{
  if (movePlayerUp)
  {
    velocity.y = -300.f;
  }
  else if (movePlayerDown)
  {
    velocity.y = 300.f;
  }
  else
  {
    velocity.y = 0.f;
  }
}

bool Player::isPenetratingTopWall(sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top < 0;
}

bool Player::isPenetratingBottomWall(sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top + shape.getGlobalBounds().height > window.getSize().y;
}

void Player::handleWallCollision(sf::RenderWindow& window)
{
  if (isPenetratingTopWall(window))
  {
    shape.setPosition(shape.getPosition().x, 0);
  }
  else if (isPenetratingBottomWall(window))
  {
    shape.setPosition(shape.getPosition().x, window.getSize().y - shape.getGlobalBounds().height);
  }
}

void Player::draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f currentPosition{shape.getPosition()};
  sf::Vector2f interpolatedPlayerPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  shape.setPosition(interpolatedPlayerPosition);
  window.draw(shape);
  shape.setPosition(currentPosition);
}

void Player::move(sf::Time timeStep)
{
  previousPosition = shape.getPosition();
  shape.move(velocity * timeStep.asSeconds());
}

void Player::reset(sf::RenderWindow& window)
{
  shape.setPosition(10, window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  velocity.x = 0.f;
  velocity.y = 0.f;
  movePlayerUp = false;
  movePlayerDown = false;
}