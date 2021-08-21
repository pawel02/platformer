#include "Game/GameState.hpp"
int main()
{
    GameState game{ 600, 900 };

    return game.gameLoop();
}