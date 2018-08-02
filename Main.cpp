#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window{sf::VideoMode{500, 500}, "Pong"};

  sf::CircleShape ball{5.f};
  ball.setPosition(100, 50);
  ball.setFillColor(sf::Color::Green);
  sf::Vector2f ballVelocity{0.1f, 0.1f};

  sf::RectangleShape player{sf::Vector2f{10.f, 100.f}};
  player.setPosition(10, 50);
  player.setFillColor(sf::Color::Red);
  sf::Vector2f playerVelocity{0.f, 0.f};
  bool movePlayerUp{false};
  bool movePlayerDown{false};

  sf::RectangleShape opponent{sf::Vector2f{10.f, 100.f}};
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width), 50);
  opponent.setFillColor(sf::Color::Blue);
  sf::Vector2f opponentVelocity{0.f, 0.f};

  // sf::Font font;
  // font.loadFromFile("C:\Windows\Fonts\Arial.ttf");
  // sf::Text text;
  // text.setString("")

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
        if (event.key.code == sf::Keyboard::Up)
        {
          movePlayerUp = true;
        }
        else if (event.key.code == sf::Keyboard::Down)
        {
          movePlayerDown = true;
        }
      }
      else if (event.type == sf::Event::KeyReleased)
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

    // Set Opponent velocity
    if (ball.getPosition().y < opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
    {
      opponentVelocity.y = -0.1f;
    }
    else if (ball.getPosition().y > opponent.getGlobalBounds().top + opponent.getGlobalBounds().height / 2)
    {
      opponentVelocity.y = 0.1f;
    }
    else
    {
      opponentVelocity.y = 0.f;
    }

    // Set Player velocity
    if (movePlayerUp)
    {
      playerVelocity.y = -0.1f;
    }
    else if (movePlayerDown)
    {
      playerVelocity.y = 0.1f;
    }
    else
    {
      playerVelocity.y = 0.f;
    }

    player.move(playerVelocity);
    ball.move(ballVelocity);
    opponent.move(opponentVelocity);

    // Handle Ball-Wall collision
    if (ball.getGlobalBounds().left < 0 && ballVelocity.x < 0 || ball.getGlobalBounds().left + ball.getGlobalBounds().width > window.getSize().x && ballVelocity.x > 0)
    {
      ballVelocity.x *= -1;
    }
    if (ball.getGlobalBounds().top < 0 && ballVelocity.y < 0 || ball.getGlobalBounds().top + ball.getGlobalBounds().height > window.getSize().y && ballVelocity.y > 0)
    {
      ballVelocity.y *= -1;
    }

    // Handle Player-Wall collision
    if (player.getGlobalBounds().top < 0)
    {
      player.setPosition(player.getPosition().x, 0);
    }
    else if (player.getGlobalBounds().top + player.getGlobalBounds().height > window.getSize().y)
    {
      player.setPosition(player.getPosition().x, window.getSize().y - player.getGlobalBounds().height);
    }

    // Handle Opponent-Wall collision
    if (opponent.getGlobalBounds().top < 0)
    {
      opponent.setPosition(opponent.getPosition().x, 0);
    }
    else if (opponent.getGlobalBounds().top + opponent.getGlobalBounds().height > window.getSize().y)
    {
      opponent.setPosition(opponent.getPosition().x, window.getSize().y - opponent.getGlobalBounds().height);
    }

    // Handle Player-Ball collision
    if (player.getGlobalBounds().intersects(ball.getGlobalBounds()) && ballVelocity.x < 0)
    {
      ballVelocity.x *= -1;
    }

    // Handle Opponent-Ball collision
    if (opponent.getGlobalBounds().intersects(ball.getGlobalBounds()) && ballVelocity.x > 0)
    {
      ballVelocity.x *= -1;
    }

    window.clear();
    window.draw(ball);
    window.draw(player);
    window.draw(opponent);
    window.display();
  }
}