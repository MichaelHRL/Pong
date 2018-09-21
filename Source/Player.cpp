#include "Player.hh"

sf::RectangleShape createPlayerShape(const sf::RenderWindow& window)
{
  sf::RectangleShape shape{sf::Vector2f{10.f, 100.f}};
  shape.setPosition(10, window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  shape.setFillColor(sf::Color::Red);
  return shape;
}


Player::Player(const sf::RenderWindow& window)
  : shape{createPlayerShape(window)}
  , velocity{0.f, 0.f}
  , previousPosition{shape.getPosition()}
  , direction{Direction::None}
{
}

sf::FloatRect Player::getGlobalAABB() const
{
  return shape.getGlobalBounds();
}

void Player::setMovementDirection(const Direction& direction)
{
  this->direction = direction;
}

void Player::setVelocity()
{
  if (direction == Direction::Up)
  {
    velocity.y = -300.f;
  }
  else if (direction == Direction::Down)
  {
    velocity.y = 300.f;
  }
  else if (direction == Direction::None)
  {
    velocity.y = 0.f;
  }
}

bool Player::isPenetratingTopWall(const sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top < 0;
}

bool Player::isPenetratingBottomWall(const sf::RenderWindow& window)
{
  return shape.getGlobalBounds().top + shape.getGlobalBounds().height > window.getSize().y;
}

void Player::handleWallCollision(const sf::RenderWindow& window)
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

void Player::draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window)
{
  const float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  const sf::Vector2f currentPosition{shape.getPosition()};
  const sf::Vector2f interpolatedPlayerPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  shape.setPosition(interpolatedPlayerPosition);
  window.draw(shape);
  shape.setPosition(currentPosition);
}

void Player::move(const sf::Time& timeStep)
{
  previousPosition = shape.getPosition();
  shape.move(velocity * timeStep.asSeconds());
}

void Player::reset(const sf::RenderWindow& window)
{
  shape.setPosition(10, window.getSize().y / 2.f - shape.getGlobalBounds().height / 2.f);
  velocity.x = 0.f;
  velocity.y = 0.f;
  direction = Direction::None;
}