#pragma once
#include <enet\enet.h>
#include "Stand.h"
#include <math.h>
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
	float GetExp(float in)
	{
		float ex = 1;
		if (in > 10)
		{
			for (int i = -1; in * powf(10,i) > 10;--i)
			{
				ex = i;
			}
		}
		if (in < -10)
		{
			for (int i = 1; in * powf(10, i) < -10;++i)
			{
				ex = i;
			}
		}
		return min(abs(ex), 100) * (abs(ex) / ex);
	}
};

