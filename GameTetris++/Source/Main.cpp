#include <iostream>

#include "Game.h"

int main(int argc, char *argv[])
{
	std::string arguments;
	
	for(int i = 0; i < argc; ++i)
	{
		arguments += argv[i];
	}

	
	Game::GetGameInstance().Run(arguments);

	return 0;
}
