#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

void resetEntities(sf::CircleShape& ball, sf::RectangleShape& player, sf::RectangleShape& opponent, sf::RenderWindow& window, sf::Vector2f& ballVelocity, sf::Vector2f& playerVelocity, sf::Vector2f& opponentVelocity, bool& movePlayerUp, bool& movePlayerDown)
{
  ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  ballVelocity.x = 424.f;
  ballVelocity.y = 0.f;

  player.setPosition(10, window.getSize().y / 2.f - player.getGlobalBounds().height / 2.f);
  playerVelocity.x = 0.f;
  playerVelocity.y = 0.f;
  movePlayerUp = false;
  movePlayerDown = false;
  
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
  opponentVelocity.x = 0.f;
  opponentVelocity.y = 0.f;
}

void setOpponentVelocity(sf::CircleShape& ball, sf::RectangleShape& opponent, sf::RenderWindow& window, sf::Vector2f& ballVelocity, sf::Vector2f& opponentVelocity)
{
  if (ball.getGlobalBounds().left > window.getSize().x / 2.f && ballVelocity.x > 0)
  {
    if (ball.getPosition().y < opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
    {
      opponentVelocity.y = -300.f;
    }
    else if (ball.getPosition().y > opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
    {
      opponentVelocity.y = 300.f;
    }
    else
    {
      opponentVelocity.y = 0.f;
    }
  }
  else
  {
    opponentVelocity.y = 0.f;
  }
}

void setPlayerVelocity(sf::Vector2f& playerVelocity, bool& movePlayerUp, bool& movePlayerDown)
{
  if (movePlayerUp)
  {
    playerVelocity.y = -300.f;
  }
  else if (movePlayerDown)
  {
    playerVelocity.y = 300.f;
  }
  else
  {
    playerVelocity.y = 0.f;
  }
}

void moveEntities(sf::Clock& clock, sf::Time& accumulator, sf::Time& timeStep, sf::RectangleShape& player, sf::RectangleShape& opponent, sf::Vector2f& playerVelocity, sf::Vector2f& ballVelocity, sf::Vector2f& previousBallPosition, sf::Vector2f& currentBallPosition, sf::CircleShape& ball, sf::Vector2f& opponentVelocity)
{
  sf::Time frameTime{clock.restart()};
  if (frameTime.asSeconds() > 0.1f)
  {
    frameTime = sf::seconds(0.1f);
  }
  accumulator += frameTime;

  while (accumulator >= timeStep)
  {
    player.move(playerVelocity * timeStep.asSeconds());

    previousBallPosition = currentBallPosition;
    ball.move(ballVelocity * timeStep.asSeconds());
    currentBallPosition = ball.getPosition();

    opponent.move(opponentVelocity * timeStep.asSeconds());

    accumulator -= timeStep;
  }
}

void handleBallWallCollision(sf::CircleShape& ball, sf::Vector2f& ballVelocity, sf::RenderWindow& window)
{
  if (ball.getGlobalBounds().left < 0 && ballVelocity.x < 0 || ball.getGlobalBounds().left + ball.getGlobalBounds().width > window.getSize().x && ballVelocity.x > 0)
  {
    ballVelocity.x *= -1;
  }
  if (ball.getGlobalBounds().top < 0 && ballVelocity.y < 0 || ball.getGlobalBounds().top + ball.getGlobalBounds().height > window.getSize().y && ballVelocity.y > 0)
  {
    ballVelocity.y *= -1;
  }
}

void handlePlayerWallCollision(sf::RectangleShape& player, sf::RenderWindow& window)
{
  if (player.getGlobalBounds().top < 0)
  {
    player.setPosition(player.getPosition().x, 0);
  }
  else if (player.getGlobalBounds().top + player.getGlobalBounds().height > window.getSize().y)
  {
    player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
  }
}

void handleOpponentWallCollision(sf::RectangleShape& opponent, sf::RenderWindow& window)
{
  if (opponent.getGlobalBounds().top < 0)
  {
    opponent.setPosition(opponent.getPosition().x, 0);
  }
  else if (opponent.getGlobalBounds().top + opponent.getGlobalBounds().height > window.getSize().y)
  {
    opponent.setPosition(opponent.getPosition().x, window.getSize().y - opponent.getGlobalBounds().height);
  }
}

void handlePlayerBallCollision(sf::RectangleShape& player, sf::CircleShape& ball, sf::Vector2f& ballVelocity)
{
  if (player.getGlobalBounds().intersects(ball.getGlobalBounds()) && ballVelocity.x < 0)
  {
    sf::Vector2f contactPoint{player.getGlobalBounds().left + player.getGlobalBounds().width, ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - (player.getGlobalBounds().left + player.getGlobalBounds().width), contactPoint.y - player.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (player.getGlobalBounds().height / 2) - 1)};
    double magnitude{std::sqrt(std::pow(ballVelocity.x, 2) + std::pow(ballVelocity.y, 2))};
    constexpr float pi{3.14159265359};
    ballVelocity.x = magnitude * std::cos(alpha * (pi / 180));
    ballVelocity.y = magnitude * std::sin(alpha * (pi / 180));
  }
}

void handleOpponenetBallCollision(sf::RectangleShape& opponent, sf::CircleShape& ball, sf::Vector2f& ballVelocity)
{
  if (opponent.getGlobalBounds().intersects(ball.getGlobalBounds()) && ballVelocity.x > 0)
  {
    sf::Vector2f contactPoint{opponent.getGlobalBounds().left, ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - opponent.getGlobalBounds().left, contactPoint.y - opponent.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (opponent.getGlobalBounds().height / 2) - 1)};
    double magnitude{std::sqrt(std::pow(ballVelocity.x, 2) + std::pow(ballVelocity.y, 2))};
    constexpr float pi{3.14159265359};
    ballVelocity.x = magnitude * std::cos(alpha * (pi / 180)) * -1;
    ballVelocity.y = magnitude * std::sin(alpha * (pi / 180));
  }
}


sf::CircleShape createBall(sf::RenderWindow& window)
{
  sf::CircleShape ball{5.f};
  ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  ball.setFillColor(sf::Color::Green);
  return ball;
}

sf::RectangleShape createPlayer(sf::RenderWindow& window)
{
  sf::RectangleShape player{sf::Vector2f{10.f, 100.f}};
  player.setPosition(10, window.getSize().y / 2.f - player.getGlobalBounds().height / 2.f);
  player.setFillColor(sf::Color::Red);
  return player;
}

sf::RectangleShape createOpponent(sf::RenderWindow& window)
{
  sf::RectangleShape opponent{sf::Vector2f{10.f, 100.f}};
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
  opponent.setFillColor(sf::Color::Blue);
  return opponent;
}

sf::Text createMenuText(sf::Font& font)
{
  sf::Text menuText;
  menuText.setFont(font);
  menuText.setString("Press space to play.");
  menuText.setFillColor(sf::Color::White);
  menuText.setCharacterSize(32);
  return menuText;
}

sf::Text createLoseText(sf::Font& font)
{
  sf::Text loseText;
  loseText.setFont(font);
  loseText.setString("You lose!\nPress space to play again.");
  loseText.setFillColor(sf::Color::White);
  loseText.setCharacterSize(32);
  return loseText;
}

sf::Text createWinText(sf::Font& font)
{
  sf::Text winText;
  winText.setFont(font);
  winText.setString("You win!\nPress space to play again.");
  winText.setFillColor(sf::Color::White);
  winText.setCharacterSize(32);
  return winText;
}

int main()
{
  sf::RenderWindow window{sf::VideoMode{500, 500}, "Pong"};

  sf::CircleShape ball{createBall(window)};
  sf::Vector2f ballVelocity{424.f, 0.f};
  sf::Vector2f previousBallPosition{ball.getPosition()};
  sf::Vector2f currentBallPosition{ball.getPosition()};
  
  sf::RectangleShape player{createPlayer(window)};
  sf::Vector2f playerVelocity{0.f, 0.f};
  bool movePlayerUp{false};
  bool movePlayerDown{false};

  sf::RectangleShape opponent{createOpponent(window)};
  sf::Vector2f opponentVelocity{0.f, 0.f};


  sf::Font font;
  if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
  {
    std::cout << "Couldn't load font" << '\n';
    return -1;
  }

  sf::Text menuText{createMenuText(font)};
  sf::Text loseText{createLoseText(font)};
  sf::Text winText{createWinText(font)};


  bool menu{true};
  bool playing{false};
  bool lose{false};
  bool win{false};


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
        if (playing)
        {
          if (event.key.code == sf::Keyboard::Up)
          {
            movePlayerUp = true;
          }
          else if (event.key.code == sf::Keyboard::Down)
          {
            movePlayerDown = true;
          }
        }
        else if (menu)
        {
          if (event.key.code == sf::Keyboard::Space)
          {
            menu = false;
            playing = true;
          }
        }
        else if (lose)
        {
          if (event.key.code == sf::Keyboard::Space)
          {
            lose = false;
            playing = true;

            resetEntities(ball, player, opponent, window, ballVelocity, playerVelocity, opponentVelocity, movePlayerUp, movePlayerUp);
          }
        }
        else if (win)
        {
          if (event.key.code == sf::Keyboard::Space)
          {
            win = false;
            playing = true;

            resetEntities(ball, player, opponent, window, ballVelocity, playerVelocity, opponentVelocity, movePlayerUp, movePlayerUp);
          }
        }
      }
      else if (event.type == sf::Event::KeyReleased)
      {
        if (playing)
        {
          if (event.key.code == sf::Keyboard::Up)
          {
            movePlayerUp = false;
          }
          else if (event.key.code == sf::Keyboard::Down)
          {
            movePlayerDown = false;
          }
        }
      }
    }

    // Set states
    if (playing) {
      if (ball.getGlobalBounds().left < 0)
      {
        lose = true;
        playing = false;
      }
      else if (ball.getGlobalBounds().left + ball.getGlobalBounds().width > window.getSize().x)
      {
        win = true;
        playing = false;
      }
    }

    if (playing)
    {
      setOpponentVelocity(ball, opponent, window, ballVelocity, opponentVelocity);
      setPlayerVelocity(playerVelocity, movePlayerUp, movePlayerDown);

      moveEntities(clock, accumulator, timeStep, player, opponent, playerVelocity, ballVelocity, previousBallPosition, currentBallPosition, ball, opponentVelocity);     

      handleBallWallCollision(ball, ballVelocity, window);
      handlePlayerWallCollision(player, window);
      handleOpponentWallCollision(opponent, window);

      handlePlayerBallCollision(player, ball, ballVelocity);
      handleOpponenetBallCollision(opponent, ball, ballVelocity);
    }

    window.clear();

    if (playing)
    {
      float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
      sf::Vector2f interpolatedBallPosition{currentBallPosition * alpha + previousBallPosition * (1.0f - alpha)};      
      ball.setPosition(interpolatedBallPosition);
      window.draw(ball);
      ball.setPosition(currentBallPosition);

      window.draw(player);
      window.draw(opponent);
    }
    else if (menu)
    {
      window.draw(menuText);
    }
    else if (lose)
    {
      window.draw(loseText);
    }
    else if (win)
    {
      window.draw(winText);
    }
    
    window.display();
  }
}