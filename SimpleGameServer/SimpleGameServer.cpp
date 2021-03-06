// SimpleHelloMyWorldServer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "SimpleGameServer.h"
#include <string>
#include <iostream>

int main()
{
	std::string exterminateChar;
	gameServerManager.Init();
	gameServerManager.Start(ServerType::LOGIN);
	while (true)
	{
		std::cin >> exterminateChar;
		if (exterminateChar.compare("q") == 0)
		{
			break;
		}
	}
	return 0;
}

void GameServerManager::Init()
{
	LoginServerObject.Init();
}

void GameServerManager::Start(ServerType type)
{
	switch (type)
	{
	case ServerType::LOGIN:
		LoginServerObject.Start();
		break;
	default:
		break;
	}
}
