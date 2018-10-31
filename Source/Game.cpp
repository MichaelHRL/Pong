#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include "Ball.hh"
#include "Player.hh"
#include "Opponent.hh"
#include "Game.hh"

enum class GameState
{
  Menu,
  Playing,
  Lose,
  Win
};

void resetEntities(Ball& ball, Player& player, Opponent& opponent, const sf::RenderWindow& window)
{
  ball.reset();
  player.reset(window);
  opponent.reset(window);
}

void moveEntities(sf::Clock& clock, sf::Time& accumulator, const sf::Time& timeStep, Player& player, Opponent& opponent, Ball& ball)
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

float toRadians(const float angleInDegrees)
{
  constexpr float pi{3.14159265359};
  constexpr float conversionScaler{pi / 180};
  return angleInDegrees * conversionScaler;
}

float vectorMagnitude(const sf::Vector2f& vector)
{
  return std::sqrt(std::pow(vector.x, 2) + std::pow(vector.y, 2));
}

// Calculates the reflected velocity of the ball from the paddle, however the angle is in the range 0 < angle < 90 || 270 < angle < 360 so this means that the x component of the reflected vector must have its x component negated when this function is used for the ball-opponent collision.
sf::Vector2f reflectedVector(const sf::FloatRect& paddle, const Ball& ball)
{
  const sf::Vector2f centerOfLineSegment{0.f, paddle.top + paddle.height / 2};
  const sf::Vector2f translatedContactPoint{0.f, ball.getPosition().y - centerOfLineSegment.y};
  const float relativeDistanceToCenter{translatedContactPoint.y / (paddle.height / 2)};
  constexpr float maxReflectedAngle{50};

  const float reflectedAngle{maxReflectedAngle * relativeDistanceToCenter};
  const float reflectedMagnitude{vectorMagnitude(ball.getVelocity())};

  float x{reflectedMagnitude * std::cos(toRadians(reflectedAngle))};
  float y{reflectedMagnitude * std::sin(toRadians(reflectedAngle))};
  return sf::Vector2f{x, y};
}

void handlePlayerBallCollision(const Player& player, Ball& ball)
{
  if (player.getGlobalAABB().intersects(ball.getGlobalAABB()) && ball.getVelocity().x < 0)
  {
    const sf::Vector2f newVelocity{reflectedVector(player.getGlobalAABB(), ball)};
    ball.setVelocity(newVelocity.x, newVelocity.y);
    ball.playAudio();
  }
}

void handleOpponentBallCollision(const Opponent& opponent, Ball& ball)
{
  if (opponent.getGlobalAABB().intersects(ball.getGlobalAABB()) && ball.getVelocity().x > 0)
  {
    const sf::Vector2f newVelocity{reflectedVector(opponent.getGlobalAABB(), ball)};
    ball.setVelocity(-newVelocity.x, newVelocity.y);
    ball.playAudio();
  }
}

sf::Text createBasicText(const sf::Font& font, const std::string& string)
{
  sf::Text text;
  text.setFont(font);
  text.setString(string);
  text.setFillColor(sf::Color::White);
  text.setCharacterSize(32);
  return text;
}

sf::Text createMenuText(const sf::Font& font)
{
  const std::string text{"Press space to play.\nUse up and down to move."};
  return createBasicText(font, text);
}

sf::Text createLoseText(const sf::Font& font)
{
  const std::string text{"You lose!\nPress space to play again.\nPress escape to exit."};
  return createBasicText(font, text);
}

sf::Text createWinText(const sf::Font& font)
{
  const std::string text{"You win!\nPress space to play again.\nPress escape to exit."};
  return createBasicText(font, text);
}

void handleKeyPressEvent(GameState& currentGameState, const sf::Event& event, Ball& ball, Player& player, Opponent& opponent, sf::RenderWindow& window)
{
  if (currentGameState == GameState::Playing)
  {
    if (event.key.code == sf::Keyboard::Up)
    {
      player.setMovementDirection(Player::Direction::Up);
    }
    else if (event.key.code == sf::Keyboard::Down)
    {
      player.setMovementDirection(Player::Direction::Down);
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

void handleKeyReleaseEvent(const GameState& currentGameState, const sf::Event& event, Player& player)
{
  if (currentGameState == GameState::Playing)
  {
    if (event.key.code == sf::Keyboard::Up)
    {
      player.setMovementDirection(Player::Direction::None);
    }
    else if (event.key.code == sf::Keyboard::Down)
    {
      player.setMovementDirection(Player::Direction::None);
    }
  }
}

void setState(GameState& currentGameState, const Ball& ball, const sf::RenderWindow& window)
{
  if (currentGameState == GameState::Playing) {
    if (ball.getGlobalAABB().left < 0)
    {
      currentGameState = GameState::Lose;
    }
    else if (ball.getGlobalAABB().left + ball.getGlobalAABB().width > window.getSize().x)
    {
      currentGameState = GameState::Win;
    }
  }
}

void startGame()
{
  sf::SoundBuffer winSoundBuffer;
  if (!winSoundBuffer.loadFromFile("../Resources/Win.wav"))
  {
    throw "Could not load Win.wav";
  }

  sf::Sound winSound{winSoundBuffer};
  bool playedWinStateAudio{false};

  sf::SoundBuffer loseSoundBuffer;
  if (!loseSoundBuffer.loadFromFile("../Resources/Lose.wav"))
  {
    throw "Could not load Lose.wav";
  }

  sf::Sound loseSound{loseSoundBuffer};
  bool playedLoseStateAudio{false};
  

  sf::RenderWindow window{sf::VideoMode{500, 500}, "Pong"};
  const sf::FloatRect playingField{0.f, 0.f, static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)};

  Ball ball{playingField};
  Player player{window};
  Opponent opponent{window};
  
  
  sf::Font font;
  if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
  {
    throw "Couldn't load font";
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

      ball.handleWallCollision();
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
      
      playedWinStateAudio = false;
      playedLoseStateAudio = false;
    }
    else if (currentGameState == GameState::Menu)
    {
      window.draw(menuText);

      playedWinStateAudio = false;
      playedLoseStateAudio = false;
    }
    else if (currentGameState == GameState::Lose)
    {
      window.draw(loseText);

      if (!playedLoseStateAudio)
      {
        loseSound.play();
        playedLoseStateAudio = true;
      }

      playedWinStateAudio = false;
    }
    else if (currentGameState == GameState::Win)
    {
      window.draw(winText);
      
      if (!playedWinStateAudio)
      {
        winSound.play();
        playedWinStateAudio = true;
      }

      playedLoseStateAudio = false;
    }
    
    window.display();
  }
}