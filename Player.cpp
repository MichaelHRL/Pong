#include "Player.hh"

sf::RectangleShape createPlayer(sf::RenderWindow& window)
{
  sf::RectangleShape player{sf::Vector2f{10.f, 100.f}};
  player.setPosition(10, window.getSize().y / 2.f - player.getGlobalBounds().height / 2.f);
  player.setFillColor(sf::Color::Red);
  return player;
}


Player::Player(sf::RenderWindow& window)
  : player{createPlayer(window)}
  , velocity{0.f, 0.f}
  , movePlayerUp{false}
  , movePlayerDown{false}
  , previousPosition{player.getPosition()}
  , currentPosition{player.getPosition()}
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
  return player.getGlobalBounds().top < 0;
}

bool Player::isPenetratingBottomWall(sf::RenderWindow& window)
{
  return player.getGlobalBounds().top + player.getGlobalBounds().height > window.getSize().y;
}

void Player::handleWallCollision(sf::RenderWindow& window)
{
  if (isPenetratingTopWall(window))
  {
    player.setPosition(player.getPosition().x, 0);
    currentPosition = player.getPosition();
  }
  else if (isPenetratingBottomWall(window))
  {
    player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
    currentPosition = player.getPosition();
  }
}

void Player::draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedPlayerPosition{currentPosition * alpha + previousPosition * (1.0f - alpha)};      
  player.setPosition(interpolatedPlayerPosition);
  window.draw(player);
  player.setPosition(currentPosition);
}

void Player::move(sf::Time timeStep)
{
  previousPosition = currentPosition;
  player.move(velocity * timeStep.asSeconds());
  currentPosition = player.getPosition();
}

void Player::reset(sf::RenderWindow& window)
{
  player.setPosition(10, window.getSize().y / 2.f - player.getGlobalBounds().height / 2.f);
  velocity.x = 0.f;
  velocity.y = 0.f;
  movePlayerUp = false;
  movePlayerDown = false;
}