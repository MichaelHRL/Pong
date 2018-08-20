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
  if (frameTime.asSeconds() > 0.1f)
  {
    frameTime = sf::seconds(0.1f);
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


void handlePlayerBallCollision(Player& player, Ball& ball)
{
  if (player.player.getGlobalBounds().intersects(ball.ball.getGlobalBounds()) && ball.velocity.x < 0)
  {
    sf::Vector2f contactPoint{player.player.getGlobalBounds().left + player.player.getGlobalBounds().width, ball.ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - (player.player.getGlobalBounds().left + player.player.getGlobalBounds().width), contactPoint.y - player.player.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (player.player.getGlobalBounds().height / 2) - 1)};
    double magnitude{std::sqrt(std::pow(ball.velocity.x, 2) + std::pow(ball.velocity.y, 2))};
    constexpr float pi{3.14159265359};
    ball.velocity.x = magnitude * std::cos(alpha * (pi / 180));
    ball.velocity.y = magnitude * std::sin(alpha * (pi / 180));
  }
}

void handleOpponentBallCollision(Opponent& opponent, Ball& ball)
{
  if (opponent.opponent.getGlobalBounds().intersects(ball.ball.getGlobalBounds()) && ball.velocity.x > 0)
  {
    sf::Vector2f contactPoint{opponent.opponent.getGlobalBounds().left, ball.ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - opponent.opponent.getGlobalBounds().left, contactPoint.y - opponent.opponent.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (opponent.opponent.getGlobalBounds().height / 2) - 1)};
    double magnitude{std::sqrt(std::pow(ball.velocity.x, 2) + std::pow(ball.velocity.y, 2))};
    constexpr float pi{3.14159265359};
    ball.velocity.x = magnitude * std::cos(alpha * (pi / 180)) * -1;
    ball.velocity.y = magnitude * std::sin(alpha * (pi / 180));
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
    if (ball.ball.getGlobalBounds().left < 0)
    {
      currentGameState = GameState::Lose;
    }
    else if (ball.ball.getGlobalBounds().left + ball.ball.getGlobalBounds().width > window.getSize().x)
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