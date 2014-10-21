#pragma once
#include "World.h"
#include <thread>
#include "ServerHost.h"
class ServerManager
{
public:
	const int PacketsPerUpdate = 10;
	int PacketsPerUpdateCounter;
	bool Running;
	ServerHost * Serverhost;
	World * world;
	int ClientCount;
	ServerManager();
	~ServerManager();
	void Update();
	void SendClientInfo();
	void MainLoop();
	void Run();
	void Init();
	void Destroy();
};

