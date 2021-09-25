#include "GamePCH.h"

#include "Game.h"

int main()
{
	Game* pGame = new Game();
	pGame->RunGame();
	
	delete pGame;

	return EXIT_SUCCESS;
}