#ifndef BALL_HH
#define BALL_HH

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

sf::CircleShape createBallShape(sf::RenderWindow& window);

class Ball
{
public:
  Ball(const sf::RenderWindow& window);

  sf::Vector2f getPosition() const;
  sf::Vector2f getVelocity() const;

  void setVelocity(float x, float y);

  sf::FloatRect getGlobalAABB() const;

  bool isPenetratingIntoLeftWall(const sf::RenderWindow& window);
  bool isPenetratingIntoRightWall(const sf::RenderWindow& window);
  bool isPenetratingIntoTopWall(const sf::RenderWindow& window);
  bool isPenetratingIntoBottomWall(const sf::RenderWindow& window);
  void handleWallCollision(const sf::RenderWindow& window);

  void draw(const sf::Time& accumulator, const sf::Time& timeStep, sf::RenderWindow& window);
  void move(const sf::Time& timeStep);
  void reset(const sf::RenderWindow& window);

  void playAudio();
private:
  sf::CircleShape shape;
  sf::Vector2f velocity;
  sf::Vector2f previousPosition;
  sf::SoundBuffer buffer;
  sf::Sound sound;
};

#endif