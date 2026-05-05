#include <FiviEngine.hpp>
#include <iostream>

int main()
{
    Game game;

	game.Start();

	game.Setup();

	game.Run();

	game.End();

    return 0;   
}