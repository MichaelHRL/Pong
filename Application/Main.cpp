#include "Game.hh"

int main()
{
  try
  {
    startGame();
  }
  catch (const std::string& errorMessage)
  {
    std::cout << errorMessage << '\n';
  }
}