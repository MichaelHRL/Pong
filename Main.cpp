#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

sf::CircleShape createBall(sf::RenderWindow& window);

class Ball
{
public:
  Ball(sf::RenderWindow& window)
    : ball{createBall(window)}
    , ballVelocity{424.f, 0.f}
    , previousBallPosition{ball.getPosition()}
    , currentBallPosition{ball.getPosition()}
  {
  }

  sf::CircleShape ball;
  sf::Vector2f ballVelocity;
  sf::Vector2f previousBallPosition;
  sf::Vector2f currentBallPosition;
};

sf::RectangleShape createPlayer(sf::RenderWindow& window);

class Player
{
public:
  Player(sf::RenderWindow& window)
    : player{createPlayer(window)}
    , playerVelocity{0.f, 0.f}
    , movePlayerUp{false}
    , movePlayerDown{false}
    , previousPlayerPosition{player.getPosition()}
    , currentPlayerPosition{player.getPosition()}
  {
  }

  sf::RectangleShape player;
  sf::Vector2f playerVelocity;
  bool movePlayerUp;
  bool movePlayerDown;
  sf::Vector2f previousPlayerPosition;
  sf::Vector2f currentPlayerPosition;
};

void resetEntities(Ball& ball, Player& player, sf::RectangleShape& opponent, sf::RenderWindow& window, sf::Vector2f& opponentVelocity)
{
  ball.ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  ball.ballVelocity.x = 424.f;
  ball.ballVelocity.y = 0.f;

  player.player.setPosition(10, window.getSize().y / 2.f - player.player.getGlobalBounds().height / 2.f);
  player.playerVelocity.x = 0.f;
  player.playerVelocity.y = 0.f;
  player.movePlayerUp = false;
  player.movePlayerDown = false;
  
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
  opponentVelocity.x = 0.f;
  opponentVelocity.y = 0.f;
}

void setOpponentVelocity(Ball& ball, sf::RectangleShape& opponent, sf::RenderWindow& window, sf::Vector2f& opponentVelocity)
{
  if (ball.ball.getGlobalBounds().left > window.getSize().x / 2.f && ball.ballVelocity.x > 0)
  {
    if (ball.ball.getPosition().y < opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
    {
      opponentVelocity.y = -300.f;
    }
    else if (ball.ball.getPosition().y > opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
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

void setPlayerVelocity(Player& player)
{
  if (player.movePlayerUp)
  {
    player.playerVelocity.y = -300.f;
  }
  else if (player.movePlayerDown)
  {
    player.playerVelocity.y = 300.f;
  }
  else
  {
    player.playerVelocity.y = 0.f;
  }
}

void moveEntities(sf::Clock& clock, sf::Time& accumulator, sf::Time& timeStep, Player& player, sf::RectangleShape& opponent, Ball& ball, sf::Vector2f& opponentVelocity, sf::Vector2f& previousOpponentPosition, sf::Vector2f& currentOpponentPosition)
{
  sf::Time frameTime{clock.restart()};
  if (frameTime.asSeconds() > 0.1f)
  {
    frameTime = sf::seconds(0.1f);
  }
  accumulator += frameTime;

  while (accumulator >= timeStep)
  {
    player.previousPlayerPosition = player.currentPlayerPosition;
    player.player.move(player.playerVelocity * timeStep.asSeconds());
    player.currentPlayerPosition = player.player.getPosition();

    ball.previousBallPosition = ball.currentBallPosition;
    ball.ball.move(ball.ballVelocity * timeStep.asSeconds());
    ball.currentBallPosition = ball.ball.getPosition();

    previousOpponentPosition = currentOpponentPosition;
    opponent.move(opponentVelocity * timeStep.asSeconds());
    currentOpponentPosition = opponent.getPosition();

    accumulator -= timeStep;
  }
}

void handleBallWallCollision(Ball& ball, sf::RenderWindow& window)
{
  if (ball.ball.getGlobalBounds().left < 0 && ball.ballVelocity.x < 0 || ball.ball.getGlobalBounds().left + ball.ball.getGlobalBounds().width > window.getSize().x && ball.ballVelocity.x > 0)
  {
    ball.ballVelocity.x *= -1;
  }
  if (ball.ball.getGlobalBounds().top < 0 && ball.ballVelocity.y < 0 || ball.ball.getGlobalBounds().top + ball.ball.getGlobalBounds().height > window.getSize().y && ball.ballVelocity.y > 0)
  {
    ball.ballVelocity.y *= -1;
  }
}

void handlePlayerWallCollision(Player& player, sf::RenderWindow& window)
{
  if (player.player.getGlobalBounds().top < 0)
  {
    player.player.setPosition(player.player.getPosition().x, 0);
    player.currentPlayerPosition = player.player.getPosition();
  }
  else if (player.player.getGlobalBounds().top + player.player.getGlobalBounds().height > window.getSize().y)
  {
    player.player.setPosition(player.player.getPosition().x, window.getSize().y - player.player.getGlobalBounds().height);
    player.currentPlayerPosition = player.player.getPosition();
  }
}

void handleOpponentWallCollision(sf::RectangleShape& opponent, sf::RenderWindow& window, sf::Vector2f& currentOpponentPosition)
{
  if (opponent.getGlobalBounds().top < 0)
  {
    opponent.setPosition(opponent.getPosition().x, 0);
    currentOpponentPosition = opponent.getPosition();
  }
  else if (opponent.getGlobalBounds().top + opponent.getGlobalBounds().height > window.getSize().y)
  {
    opponent.setPosition(opponent.getPosition().x, window.getSize().y - opponent.getGlobalBounds().height);
    currentOpponentPosition = opponent.getPosition();
  }
}

void handlePlayerBallCollision(Player& player, Ball& ball)
{
  if (player.player.getGlobalBounds().intersects(ball.ball.getGlobalBounds()) && ball.ballVelocity.x < 0)
  {
    sf::Vector2f contactPoint{player.player.getGlobalBounds().left + player.player.getGlobalBounds().width, ball.ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - (player.player.getGlobalBounds().left + player.player.getGlobalBounds().width), contactPoint.y - player.player.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (player.player.getGlobalBounds().height / 2) - 1)};
    double magnitude{std::sqrt(std::pow(ball.ballVelocity.x, 2) + std::pow(ball.ballVelocity.y, 2))};
    constexpr float pi{3.14159265359};
    ball.ballVelocity.x = magnitude * std::cos(alpha * (pi / 180));
    ball.ballVelocity.y = magnitude * std::sin(alpha * (pi / 180));
  }
}

void handleOpponentBallCollision(sf::RectangleShape& opponent, Ball& ball)
{
  if (opponent.getGlobalBounds().intersects(ball.ball.getGlobalBounds()) && ball.ballVelocity.x > 0)
  {
    sf::Vector2f contactPoint{opponent.getGlobalBounds().left, ball.ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - opponent.getGlobalBounds().left, contactPoint.y - opponent.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (opponent.getGlobalBounds().height / 2) - 1)};
    double magnitude{std::sqrt(std::pow(ball.ballVelocity.x, 2) + std::pow(ball.ballVelocity.y, 2))};
    constexpr float pi{3.14159265359};
    ball.ballVelocity.x = magnitude * std::cos(alpha * (pi / 180)) * -1;
    ball.ballVelocity.y = magnitude * std::sin(alpha * (pi / 180));
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

void drawBall(Ball& ball, sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedBallPosition{ball.currentBallPosition * alpha + ball.previousBallPosition * (1.0f - alpha)};      
  ball.ball.setPosition(interpolatedBallPosition);
  window.draw(ball.ball);
  ball.ball.setPosition(ball.currentBallPosition);
}

void drawPlayer(Player& player, sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{

  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedPlayerPosition{player.currentPlayerPosition * alpha + player.previousPlayerPosition * (1.0f - alpha)};      
  player.player.setPosition(interpolatedPlayerPosition);
  window.draw(player.player);
  player.player.setPosition(player.currentPlayerPosition);
}

void drawOpponent(sf::RectangleShape& opponent, sf::Vector2f& currentOpponentPosition, sf::Vector2f& previousOpponentPosition, sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
{
  float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
  sf::Vector2f interpolatedOpponentPosition{currentOpponentPosition * alpha + previousOpponentPosition * (1.0f - alpha)};      
  opponent.setPosition(interpolatedOpponentPosition);
  window.draw(opponent);
  opponent.setPosition(currentOpponentPosition);
}


void handleKeyPressEvent(bool& playing, bool& menu, bool& lose, bool& win, sf::Event& event, Ball& ball, Player& player, sf::RectangleShape& opponent, sf::RenderWindow& window, sf::Vector2f& opponentVelocity)
{
  if (playing)
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

      resetEntities(ball, player, opponent, window, opponentVelocity);
    }
  }
  else if (win)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      win = false;
      playing = true;

      resetEntities(ball, player, opponent, window, opponentVelocity);
    }
  }
}

void handleKeyReleaseEvent(bool& playing, sf::Event& event, Player& player)
{
  if (playing)
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

void setState(bool& playing, bool& lose, bool& win, Ball& ball, sf::RenderWindow& window)
{
  if (playing) {
    if (ball.ball.getGlobalBounds().left < 0)
    {
      lose = true;
      playing = false;
    }
    else if (ball.ball.getGlobalBounds().left + ball.ball.getGlobalBounds().width > window.getSize().x)
    {
      win = true;
      playing = false;
    }
  }
}



int main()
{
  sf::RenderWindow window{sf::VideoMode{500, 500}, "Pong"};

  Ball ball{window};
  
  Player player{window};

  sf::RectangleShape opponent{createOpponent(window)};
  sf::Vector2f opponentVelocity{0.f, 0.f};
  sf::Vector2f previousOpponentPosition{opponent.getPosition()};
  sf::Vector2f currentOpponentPosition{opponent.getPosition()};


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
        handleKeyPressEvent(playing, menu, lose, win, event, ball, player, opponent, window, opponentVelocity);
      }
      else if (event.type == sf::Event::KeyReleased)
      {
        handleKeyReleaseEvent(playing, event, player);
      }
    }

    setState(playing, lose, win, ball, window);

    if (playing)
    {
      setOpponentVelocity(ball, opponent, window, opponentVelocity);
      setPlayerVelocity(player);

      moveEntities(clock, accumulator, timeStep, player, opponent, ball, opponentVelocity, previousOpponentPosition, currentOpponentPosition);     

      handleBallWallCollision(ball, window);
      handlePlayerWallCollision(player, window);
      handleOpponentWallCollision(opponent, window, currentOpponentPosition);

      handlePlayerBallCollision(player, ball);
      handleOpponentBallCollision(opponent, ball);
    }

    window.clear();

    if (playing)
    {
      drawBall(ball, accumulator, timeStep, window);
      drawPlayer(player, accumulator, timeStep, window);
      drawOpponent(opponent, currentOpponentPosition, previousOpponentPosition, accumulator, timeStep, window);
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