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

  bool isPenetratingIntoLeftWall(sf::RenderWindow& window)
  {
    // The check for ballVelocity is neccessary because it ensures that the ball doesn't stay trapped within a surface
    return ball.getGlobalBounds().left < 0 && ballVelocity.x < 0;
  }

  bool isPenetratingIntoRightWall(sf::RenderWindow& window)
  {
    return ball.getGlobalBounds().left + ball.getGlobalBounds().width > window.getSize().x && ballVelocity.x > 0;
  }

  bool isPenetratingIntoTopWall(sf::RenderWindow& window)
  {
    return ball.getGlobalBounds().top < 0 && ballVelocity.y < 0;
  }

  bool isPenetratingIntoBottomWall(sf::RenderWindow& window)
  {
    return ball.getGlobalBounds().top + ball.getGlobalBounds().height > window.getSize().y && ballVelocity.y > 0;
  }

  void handleWallCollision(sf::RenderWindow& window)
  {
    if (isPenetratingIntoLeftWall(window) || isPenetratingIntoRightWall(window))
    {
      ballVelocity.x *= -1;
    }
    if (isPenetratingIntoTopWall(window) || isPenetratingIntoBottomWall(window))
    {
      ballVelocity.y *= -1;
    }
  }

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
  {
    float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
    sf::Vector2f interpolatedBallPosition{currentBallPosition * alpha + previousBallPosition * (1.0f - alpha)};      
    ball.setPosition(interpolatedBallPosition);
    window.draw(ball);
    ball.setPosition(currentBallPosition);
  }
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

  void setVelocity()
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

  bool isPenetratingTopWall(sf::RenderWindow& window)
  {
    return player.getGlobalBounds().top < 0;
  }

  bool isPenetratingBottomWall(sf::RenderWindow& window)
  {
    return player.getGlobalBounds().top + player.getGlobalBounds().height > window.getSize().y;
  }

  void handleWallCollision(sf::RenderWindow& window)
  {
    if (isPenetratingTopWall(window))
    {
      player.setPosition(player.getPosition().x, 0);
      currentPlayerPosition = player.getPosition();
    }
    else if (isPenetratingBottomWall(window))
    {
      player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
      currentPlayerPosition = player.getPosition();
    }
  }

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
  {
    float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
    sf::Vector2f interpolatedPlayerPosition{currentPlayerPosition * alpha + previousPlayerPosition * (1.0f - alpha)};      
    player.setPosition(interpolatedPlayerPosition);
    window.draw(player);
    player.setPosition(currentPlayerPosition);
  }
};

sf::RectangleShape createOpponent(sf::RenderWindow& window);

class Opponent
{
public:
  Opponent(sf::RenderWindow& window)
    : opponent{createOpponent(window)}
    , opponentVelocity{0.f, 0.f}
    , previousOpponentPosition{opponent.getPosition()}
    , currentOpponentPosition{opponent.getPosition()}
  {
  }

  sf::RectangleShape opponent;
  sf::Vector2f opponentVelocity;
  sf::Vector2f previousOpponentPosition;
  sf::Vector2f currentOpponentPosition;

  void setVelocity(Ball& ball, sf::RenderWindow& window)
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

  void handleWallCollision(sf::RenderWindow& window)
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

  void draw(sf::Time& accumulator, sf::Time& timeStep, sf::RenderWindow& window)
  {
    float alpha{accumulator.asSeconds() / timeStep.asSeconds()};
    sf::Vector2f interpolatedOpponentPosition{currentOpponentPosition * alpha + previousOpponentPosition * (1.0f - alpha)};      
    opponent.setPosition(interpolatedOpponentPosition);
    window.draw(opponent);
    opponent.setPosition(currentOpponentPosition);
  }
};

class GameStates
{
public:
  GameStates(bool menu, bool playing, bool lose, bool win)
    : menu{menu}
    , playing{playing}
    , lose{lose}
    , win{win}
  {
  }

  bool menu{true};
  bool playing{false};
  bool lose{false};
  bool win{false};
};

void resetEntities(Ball& ball, Player& player, Opponent& opponent, sf::RenderWindow& window)
{
  ball.ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  ball.ballVelocity.x = 424.f;
  ball.ballVelocity.y = 0.f;

  player.player.setPosition(10, window.getSize().y / 2.f - player.player.getGlobalBounds().height / 2.f);
  player.playerVelocity.x = 0.f;
  player.playerVelocity.y = 0.f;
  player.movePlayerUp = false;
  player.movePlayerDown = false;
  
  opponent.opponent.setPosition(window.getSize().x - (10 + opponent.opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.opponent.getGlobalBounds().height / 2.f);
  opponent.opponentVelocity.x = 0.f;
  opponent.opponentVelocity.y = 0.f;
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
    player.previousPlayerPosition = player.currentPlayerPosition;
    player.player.move(player.playerVelocity * timeStep.asSeconds());
    player.currentPlayerPosition = player.player.getPosition();

    ball.previousBallPosition = ball.currentBallPosition;
    ball.ball.move(ball.ballVelocity * timeStep.asSeconds());
    ball.currentBallPosition = ball.ball.getPosition();

    opponent.previousOpponentPosition = opponent.currentOpponentPosition;
    opponent.opponent.move(opponent.opponentVelocity * timeStep.asSeconds());
    opponent.currentOpponentPosition = opponent.opponent.getPosition();

    accumulator -= timeStep;
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

void handleOpponentBallCollision(Opponent& opponent, Ball& ball)
{
  if (opponent.opponent.getGlobalBounds().intersects(ball.ball.getGlobalBounds()) && ball.ballVelocity.x > 0)
  {
    sf::Vector2f contactPoint{opponent.opponent.getGlobalBounds().left, ball.ball.getPosition().y};
    sf::Vector2f translatedContactPoint{contactPoint.x - opponent.opponent.getGlobalBounds().left, contactPoint.y - opponent.opponent.getGlobalBounds().top};
    
    float maxAngle{50};
    float alpha{maxAngle * (translatedContactPoint.y / (opponent.opponent.getGlobalBounds().height / 2) - 1)};
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

void handleKeyPressEvent(GameStates& gameStates, sf::Event& event, Ball& ball, Player& player, Opponent& opponent, sf::RenderWindow& window)
{
  if (gameStates.playing)
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
  else if (gameStates.menu)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      gameStates.menu = false;
      gameStates.playing = true;
    }
  }
  else if (gameStates.lose)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      gameStates.lose = false;
      gameStates.playing = true;

      resetEntities(ball, player, opponent, window);
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
      window.close();
    }
  }
  else if (gameStates.win)
  {
    if (event.key.code == sf::Keyboard::Space)
    {
      gameStates.win = false;
      gameStates.playing = true;

      resetEntities(ball, player, opponent, window);
    }
    else if (event.key.code == sf::Keyboard::Escape)
    {
      window.close();
    }
  }
}

void handleKeyReleaseEvent(GameStates& gameStates, sf::Event& event, Player& player)
{
  if (gameStates.playing)
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

void setState(GameStates& gameStates, Ball& ball, sf::RenderWindow& window)
{
  if (gameStates.playing) {
    if (ball.ball.getGlobalBounds().left < 0)
    {
      gameStates.lose = true;
      gameStates.playing = false;
    }
    else if (ball.ball.getGlobalBounds().left + ball.ball.getGlobalBounds().width > window.getSize().x)
    {
      gameStates.win = true;
      gameStates.playing = false;
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

  GameStates gameStates{true, false, false, false};

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
        handleKeyPressEvent(gameStates, event, ball, player, opponent, window);
      }
      else if (event.type == sf::Event::KeyReleased)
      {
        handleKeyReleaseEvent(gameStates, event, player);
      }
    }

    setState(gameStates, ball, window);

    if (gameStates.playing)
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

    if (gameStates.playing)
    {
      ball.draw(accumulator, timeStep, window);
      player.draw(accumulator, timeStep, window);
      opponent.draw(accumulator, timeStep, window);
    }
    else if (gameStates.menu)
    {
      window.draw(menuText);
    }
    else if (gameStates.lose)
    {
      window.draw(loseText);
    }
    else if (gameStates.win)
    {
      window.draw(winText);
    }
    
    window.display();
  }
}