#pragma once

#include "resource.h"
#include "MainServer.h"

enum class ServerType{
	MAIN = 0,
	COUNT
};
class GameServerManager {
public:
	void Init();
	void Start(ServerType type);
private:
	MainServer mainServer;
};