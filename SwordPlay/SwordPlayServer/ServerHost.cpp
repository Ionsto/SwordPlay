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
			player = Player();
			player.Id = sm->world->Players.size();
			player.Peer = event.peer;
			sm->world->Players.push_back(player);
			sm->world->SpawnPlayer(player.Id);
			for (int i = 0; i < 10; ++i)
				SendCommand(event.peer, Sword_PlayerIds, new int[] {i,player.BodyPartIds[i]}, 2);
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
		if (Id > 0 && Id < sm->world->ObjectCount)
		{
			if (sm->world->ObjectArray[Id] != NULL)
			{
				//If the incoming dx actualy changes somthing, override movement
				for (int i = 0; i < 3; ++i){
					if (event.packet->data[i + 2] != 0){
						sm->world->ObjectArray[Id]->QuedMovePos[i] = event.packet->data[i + 2] / 10;
					}
					if (event.packet->data[i + 5] != 0){
						sm->world->ObjectArray[Id]->QuedMoveRot[i] = event.packet->data[i + 5] / 10;
					}
				}
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
		for (int o = 0; o < sm->world->ObjectCount; ++o)
		{
			if (sm->world->ObjectArray[o] != NULL)
			{
				neV3 pos = sm->world->ObjectArray[o]->PhysicsBody->GetPos();
				neV3 rot = sm->world->ObjectArray[o]->RotationEuler;
				int args[] = { (int)o, (int)pos[0] * 10, (int)pos[1] * 10, (int)pos[2] * 10, (int)rot[0] * 10, (int)rot[1] * 10, (int)rot[2] * 10, 0, 0 };
				SendCommand(sm->world->Players[i].Peer, Sword_Object, (int *)args, 9);
			}
		}
	}
}