#include <SFML/Graphics.hpp>

int main()
{
  sf::RenderWindow window{sf::VideoMode{500, 500}, "SFML works!"};

  sf::CircleShape ball{5.f};
  ball.setFillColor(sf::Color::Green);
  sf::Vector2f ballVelocity{0.1f, 0.1f};

  sf::RectangleShape player{sf::Vector2f{10.f, 100.f}};
  player.setPosition(10, 50);
  player.setFillColor(sf::Color::Red);
  sf::Vector2f playerVelocity{0.f, 0.f};
  bool movePlayerUp{false};
  bool movePlayerDown{false};

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

    window.clear();
    window.draw(ball);
    window.draw(player);
    window.display();
  }
}