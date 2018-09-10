#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Ball.hh"
#include "Player.hh"
#include "Opponent.hh"

enum class GameState
{
  Menu,
  Playing,
  Lose,
  Win
};

void resetEntities(Ball& ball, Player& player, Opponent& opponent, sf::RenderWindow& window)
{
  ball.reset(window);
  player.reset(window);
  opponent.reset(window);
}

void moveEntities(sf::Clock& clock, sf::Time& accumulator, sf::Time& timeStep, Player& player, Opponent& opponent, Ball& ball)
{
  sf::Time frameTime{clock.restart()};
  // This is needed because enourmous frameTimes can lead to large, unwanted updates in the physics calculations e.g. the ball could tunnel through a paddle
  constexpr float maxFrameTime{0.1f};
  if (frameTime.asSeconds() > maxFrameTime)
  {
    frameTime = sf::seconds(maxFrameTime);
  }
  accumulator += frameTime;

  while (accumulator >= timeStep)
  {
    player.move(timeStep);
    ball.move(timeStep);
    opponent.move(timeStep);

    accumulator -= timeStep;
  }
}

float toRadians(float angleInDegrees)
{
  constexpr float pi{3.14159265359};
  constexpr float conversionScaler{pi / 180};
  return angleInDegrees * conversionScaler;
}

float vectorMagnitude(const sf::Vector2f& vector)
{
  return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2));
}

// Calculates the reflected direction of the ball from the paddle.
sf::Vector2f reflectedVector(sf::RectangleShape& paddle, Ball& ball)
{
  const sf::Vector2f centerOfLineSegment{0.f, paddle.getGlobalBounds().top + paddle.getGlobalBounds().height / 2};
  const sf::Vector2f translatedContactPoint{0.f, ball.shape.getPosition().y - centerOfLineSegment.y};
  const float relativeDistanceToCenter{translatedContactPoint.y / (paddle.getGlobalBounds().height / 2)};
  constexpr float maxReflectedAngle{50};

  const float reflectedAngle{maxReflectedAngle * relativeDistanceToCenter};
  const float reflectedMagnitude{vectorMagnitude(ball.velocity)};

  float x{reflectedMagnitude * std::cos(toRadians(reflectedAngle))};
  float y{reflectedMagnitude * std::sin(toRadians(reflectedAngle))};
  return sf::Vector2f{x, y};
}

void handlePlayerBallCollision(Player& player, Ball& ball)
{
  if (player.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()) && ball.velocity.x < 0)
  {
    ball.velocity = reflectedVector(player.shape, ball);
  }
}

void handleOpponentBallCollision(Opponent& opponent, Ball& ball)
{
  if (opponent.shape.getGlobalBounds().intersects(ball.shape.getGlobalBounds()) && ball.velocity.x > 0)
  {
    ball.velocity = reflectedVector(opponent.shape, ball);
    ball.velocity.x *= -1;
  }
}

sf::Text createMenuText(sf::Font& font)
{
  sf::Text menuText;
  menuText.setFont(font);
  menuText.setString("Press space to play.\nUse up and down to move.");
  menuText.setFillColor(sf::Color::White);
  menuText.setCharacterSize(32);
  return menuText;
}

sf::Text createLoseText(sf::Font& font)
{
  sf::Text loseText;
  loseText.setFont(font);
  loseText.setString("You lose!\nPress space to play again.\nPress escape to exit.");
  loseText.setFillColor(sf::Color::White);
  loseText.setCharacterSize(32);
  return loseText;
}

sf::Text createWinText(sf::Font& font)
{
  sf::Text winText;
  winText.setFont(font);
  winText.setString("You win!\nPress space to play again.\nPress escape to exit.");
  winText.setFillColor(sf::Color::White);
  winText.setCharacterSize(32);
  return winText;
}

void handleKeyPressEvent(GameState& currentGameState, sf::Event& event, Ball& ball, Player& player, Opponent& opponent, sf::RenderWindow& window)
{
  if (currentGameState == GameState::Playing)
  {
    if (event.key.code == sf::Keyboard::Up)
    {
      player.movePlayerUp = true;
    }
    else if (event.key.code == sf::Keyboard::Down)
    {
      player.movePlayerDown = true;
    }
  }
  else if (currentGameState == GameState::Menu)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      currentGameState = GameState::Playing;
    }
  }
  else if (currentGameState == GameState::Lose)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      currentGameState = GameState::Playing;

      resetEntities(ball, player, opponent, window);
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
      window.close();
    }
  }
  else if (currentGameState == GameState::Win)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      currentGameState = GameState::Playing;

      resetEntities(ball, player, opponent, window);
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
      window.close();
    }
  }
}

void handleKeyReleaseEvent(GameState& currentGameState, sf::Event& event, Player& player)
{
  if (currentGameState == GameState::Playing)
  {
    if (event.key.code == sf::Keyboard::Up)
    {
      player.movePlayerUp = false;
    }
    else if (event.key.code == sf::Keyboard::Down)
    {
      player.movePlayerDown = false;
    }
  }
}

void setState(GameState& currentGameState, Ball& ball, sf::RenderWindow& window)
{
  if (currentGameState == GameState::Playing) {
    if (ball.shape.getGlobalBounds().left < 0)
    {
      currentGameState = GameState::Lose;
    }
    else if (ball.shape.getGlobalBounds().left + ball.shape.getGlobalBounds().width > window.getSize().x)
    {
      currentGameState = GameState::Win;
    }
  }
}

int main()
{
  sf::RenderWindow window{sf::VideoMode{500, 500}, "Pong"};

  Ball ball{window};
  Player player{window};
  Opponent opponent{window};


  sf::Font font;
  if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
  {
    std::cout << "Couldn't load font" << '\n';
    return -1;
  }

  sf::Text menuText{createMenuText(font)};
  sf::Text loseText{createLoseText(font)};
  sf::Text winText{createWinText(font)};

  GameState currentGameState{GameState::Menu};

  sf::Clock clock{};
  sf::Time accumulator{};

  // Simulation runs at 100fps
  sf::Time timeStep{sf::seconds(0.01f)};


  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      else if (event.type == sf::Event::KeyPressed)
      {
        handleKeyPressEvent(currentGameState, event, ball, player, opponent, window);
      }
      else if (event.type == sf::Event::KeyReleased)
      {
        handleKeyReleaseEvent(currentGameState, event, player);
      }
    }

    setState(currentGameState, ball, window);

    if (currentGameState == GameState::Playing)
    {
      opponent.setVelocity(ball, window);
      player.setVelocity();

      moveEntities(clock, accumulator, timeStep, player, opponent, ball);     

      ball.handleWallCollision(window);
      player.handleWallCollision(window);
      opponent.handleWallCollision(window);

      handlePlayerBallCollision(player, ball);
      handleOpponentBallCollision(opponent, ball);
    }

    window.clear();

    if (currentGameState == GameState::Playing)
    {
      ball.draw(accumulator, timeStep, window);
      player.draw(accumulator, timeStep, window);
      opponent.draw(accumulator, timeStep, window);
    }
    else if (currentGameState == GameState::Menu)
    {
      window.draw(menuText);
    }
    else if (currentGameState == GameState::Lose)
    {
      window.draw(loseText);
    }
    else if (currentGameState == GameState::Win)
    {
      window.draw(winText);
    }
    
    window.display();
  }
}