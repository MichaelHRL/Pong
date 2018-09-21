#ifndef PLAYER_HH
#define PLAYER_HH

#include <SFML/Graphics.hpp>

sf::RectangleShape createPlayerShape(const sf::RenderWindow& window);

class Player
{
public:
  enum class Direction
  {
    Up,
    Down,
    None
  };

  Player(const sf::RenderWindow& window);

  sf::FloatRect getGlobalAABB() const;
  void setMovementDirection(const Direction& direction);

  void setVelocity();

  bool isPenetratingTopWall(const sf::RenderWindow& window);
  bool isPenetratingBottomWall(const sf::RenderWindow& window);
  void handleWallCollision(const sf::RenderWindow& window);

  void draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window);
  void move(const sf::Time& timeStep);
  void reset(const sf::RenderWindow& window);
private:
  sf::RectangleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;

  Direction direction;
};

#endif