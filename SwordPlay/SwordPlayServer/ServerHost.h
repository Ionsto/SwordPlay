#pragma once
#include <enet\enet.h>
#include "Player.h"
#include "World.h"
#include <..\SwordPlay\Stand.h>
#include <vector>
class ServerManager;
class ServerHost
{
public:
	ENetAddress address;
	ENetHost *server;
	ServerHost();
	~ServerHost();
	int PacketCount;
	void Start(int port);
	void GetMessages(ServerManager * sm);
	void ParsePacket(ServerManager * sm, ENetEvent event);
	void UpdateAll(ServerManager * sm);
	void SendCommand(ENetPeer * peer, int Command, int * Args, int argscount);
};

