#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Ball.hh"
#include "Player.hh"
#include "Opponent.hh"

enum class GameState;

void resetEntities(Ball& ball, Player& player, Opponent& opponent, const sf::RenderWindow& window);
void moveEntities(sf::Clock& clock, sf::Time& accumulator, const sf::Time& timeStep, Player& player, Opponent& opponent, Ball& ball);

float toRadians(const float angleInDegrees);
float vectorMagnitude(const sf::Vector2f& vector);

sf::Vector2f reflectedVector(const sf::RectangleShape& paddle, Ball& ball);

void handlePlayerBallCollision(const Player& player, Ball& ball);
void handleOpponentBallCollision(const Opponent& opponent, Ball& ball);

sf::Text createMenuText(const sf::Font& font);
sf::Text createLoseText(const sf::Font& font);
sf::Text createWinText(const sf::Font& font);

void handleKeyPressEvent(GameState& currentGameState, const sf::Event& event, Ball& ball, Player& player, Opponent& opponent, sf::RenderWindow& window);
void handleKeyReleaseEvent(const GameState& currentGameState, const sf::Event& event, Player& player);

void setState(GameState& currentGameState, const Ball& ball, const sf::RenderWindow& window);

void startGame();