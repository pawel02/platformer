#include "Game/GameState.hpp"
int main()
{
    GameState game{ 800, 600 };

    return game.gameLoop();
}