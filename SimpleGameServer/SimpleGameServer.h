#pragma once

#include "resource.h"
#include "LoginServer.h"

enum class ServerType{
	GAME = 0,
	LOGIN = 1,
	COUNT
};
class GameServerManager {
public:
	void Init();
	void Start(ServerType type);
private:
	LoginServer LoginServerObject;
};

// globals.
GameServerManager gameServerManager;