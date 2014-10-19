#include "ServerHost.h"
#include <iostream>
#include "ServerManager.h"
#include <vector>

ServerHost::ServerHost()
{
}


ServerHost::~ServerHost()
{
	enet_host_destroy(server);
}

void ServerHost::GetMessages(ServerManager * sm)
{
	ENetEvent event;
	std::vector<ENetEvent> Recivevents;
	while (enet_host_service(server, &event, 0) > 0)
	{
		Player player;
		int ClientID = 0;
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			/*for (int i = 0; i < sm->world->WorldSystemSize;++i)
			{
				SendWorldPart(sm,i,event.peer);
			}
			player = Player();
			player.Id = sm->world->Players.size();
			SendCommand(event.peer, RTS_PLAYERID, new int[] {player.Id}, 1);
			player.Peer = event.peer;*/
			sm->world->Players.push_back(player);
			std::cout << "Player connected\n";
			break;
		case ENET_EVENT_TYPE_RECEIVE:
			Recivevents.push_back(event);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			/*for (int i = 0; i < sm->world->Players.size(); ++i)
			{
				if (event.peer->address.host == sm->world->Players.at(i).Peer->address.host)
				{
					sm->world->Players.erase(sm->world->Players.begin() + i);
					std::cout << "player:" << i << " disconnected\n";
				}
			}*/
			event.peer->data = NULL;
		}
	}
	for (int i = 0; i < Recivevents.size(); ++i)
	{
		ParsePacket(sm,Recivevents.at(i));
		enet_packet_destroy(Recivevents.at(i).packet);
	}
}
void ServerHost::SendCommand(ENetPeer * peer, int Command, int * Args,int argscount)
{
	ENetPacket *packet;
	int * buffer = new int[argscount + 1];
	buffer[0] = Command;
	for (int i = 0; i < argscount; ++i)
	{
		buffer[i + 1] = Args[i];
	}
	packet = enet_packet_create(buffer, argscount + 1, 0);
	enet_peer_send(peer, 0, packet);
	//delete Args;
}
void ServerHost::ParsePacket(ServerManager * sm,ENetEvent event)
{
	if (event.packet->data[0] == Sword_MoveObject)
	{
		int Id = event.packet->data[1];
		int dX = event.packet->data[2];
		int dY = event.packet->data[3];
		int dZ = event.packet->data[4];
		int dRX = event.packet->data[2];
		int dRY = event.packet->data[3];
		int dRZ = event.packet->data[4];
		if (Id > 0 && Id < sm->world->ObjectCount)
		{
			if (sm->world->ObjectArray[Id] != NULL)
			{
			}
		}
	}
}
void ServerHost::Start(int port)
{
	address.host = ENET_HOST_ANY;
	address.port = port;

	server = enet_host_create(&address, 100, 2, 0, 0);

	if (server == NULL) {
		std::cout<<"Could not start server.\n";
		return;
	}
}
void ServerHost::UpdateAll(ServerManager * sm)
{
	for (int i = 0; i < sm->world->Players.size(); ++i)
	{
		for (int i = 0; i < sm->world->ObjectCount; ++i)
		{

		}
	}
}