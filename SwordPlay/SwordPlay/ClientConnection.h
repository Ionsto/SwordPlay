#pragma once
#include <enet\enet.h>
#include "Stand.h"
class GameManager;
class ClientConnection
{
public:
	ENetHost * client;
	ENetAddress address;
	ENetEvent event;
	ENetPeer *Server;
	ClientConnection();
	~ClientConnection();
	void Connect(char * ip, int port);
	void SendCommands(int Command, enet_uint8 * Args, int argscount);
	void GetInfomation(GameManager * gm);
	void Disconnect();
	void Update(GameManager * gm);
	void ParsePacket(GameManager * gm,ENetEvent event);
};

