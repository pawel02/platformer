#include "Game/GameState.hpp"
int main()
{
    GameState game{ 800, 900 };

    return game.gameLoop();
}