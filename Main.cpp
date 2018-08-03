#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

int main()
{
  sf::RenderWindow window{sf::VideoMode{500, 500}, "Pong"};

  sf::CircleShape ball{5.f};
  ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
  ball.setFillColor(sf::Color::Green);
  sf::Vector2f ballVelocity{0.1f, 0.1f};

  sf::RectangleShape player{sf::Vector2f{10.f, 100.f}};
  player.setPosition(10, window.getSize().y / 2.f - player.getGlobalBounds().height / 2.f);
  player.setFillColor(sf::Color::Red);
  sf::Vector2f playerVelocity{0.f, 0.f};
  bool movePlayerUp{false};
  bool movePlayerDown{false};

  sf::RectangleShape opponent{sf::Vector2f{10.f, 100.f}};
  opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
  opponent.setFillColor(sf::Color::Blue);
  sf::Vector2f opponentVelocity{0.f, 0.f};


  sf::Font font;
  if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf"))
  {
    std::cout << "Couldn't load font" << '\n';
    return -1;
  }

  sf::Text menuText;
  menuText.setFont(font);
  menuText.setString("Press space to play.");
  menuText.setFillColor(sf::Color::White);
  menuText.setCharacterSize(32);

  sf::Text loseText;
  loseText.setFont(font);
  loseText.setString("You lose!\nPress space to play again.");
  loseText.setFillColor(sf::Color::White);
  loseText.setCharacterSize(32);


  bool menu{true};
  bool playing{false};
  bool lose{false};


  std::random_device randomDevice;
  std::mt19937 generator{randomDevice()};
  std::uniform_real_distribution<float> distribution{0, 1};

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

            // Reset all entities' positions and velocities
            ball.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);
            ballVelocity.x = 0.1f;
            ballVelocity.y = 0.1f;

            player.setPosition(10, window.getSize().y / 2.f - player.getGlobalBounds().height / 2.f);
            playerVelocity.x = 0.f;
            playerVelocity.y = 0.f;
            movePlayerUp = false;
            movePlayerDown = false;
            
            opponent.setPosition(window.getSize().x - (10 + opponent.getGlobalBounds().width),  window.getSize().y / 2.f - opponent.getGlobalBounds().height / 2.f);
            opponentVelocity.x = 0.f;
            opponentVelocity.y = 0.f;
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

    if (playing) {
      if (ball.getGlobalBounds().left < 0)
      {
        lose = true;
        playing = false;
      }
    }

    if (playing)
    {
      // Set Opponent velocity
      // This doesn't create the AI I was hoping for
      // if (distribution(generator) > 0.4f && ball.getGlobalBounds().left > window.getSize().x / 2.f && ballVelocity.x > 0)
      if (ball.getGlobalBounds().left > window.getSize().x / 2.f && ballVelocity.x > 0)
      {
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
    }

    window.clear();

    if (playing)
    {
      window.draw(ball);
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
    
    window.display();
  }
}