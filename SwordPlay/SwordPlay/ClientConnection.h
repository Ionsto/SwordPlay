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
		const int Lim = 998001;//999 * 999
		if (abs(in) < Lim){
			float ex = 1;
			if (abs(in) > 10)
			{
				for (int i = -1; abs(in * powf(10, i)) > 10; --i)
				{
					ex = -i;
				}
			}
			return min(abs(ex), 999) * (abs(ex) / ex);
		}
		else
		{
			return 999 * (abs(in) / -in);
		}
	}
};

