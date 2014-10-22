#include "ServerHost.h"
#include <iostream>
#include "ServerManager.h"
#include <vector>

ServerHost::ServerHost()
{
	PacketCount = 0;
}


ServerHost::~ServerHost()
{
	enet_host_destroy(server);
}

void ServerHost::GetMessages(ServerManager * sm)
{
	ENetEvent event;
	while (enet_host_service(server, &event, 0) > 0)
	{
		int ClientID = 0;
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
			{
				Player * player = new Player();
				int id = sm->world->Players.size();
				player->Id = id;
				player->Peer = *event.peer;
				//sm->world->SpawnPlayer(id);
				sm->world->Players.push_back(player);
				int * Args = new int[3];
				Args[0] = Sword_PlayerIds;
				for (int i = 0; i < 10; ++i)
				{
					Args[1] = i; Args[2] = sm->world->Players[id]->BodyPartIds[i];
					ENetPacket * packet = enet_packet_create(Args, 3, 0);
					enet_peer_send(event.peer, 0, packet);
					//enet_packet_destroy(packet);
				}
				enet_host_flush(server);
				delete [] Args;
				std::cout << "Player connected\n";
				break;
			}
			case ENET_EVENT_TYPE_RECEIVE:
			{
				ParsePacket(sm, event);
				enet_packet_destroy(event.packet);
				break;
			}
			case ENET_EVENT_TYPE_DISCONNECT:
			{
				for (int i = 0; i < sm->world->Players.size(); ++i)
				{
					if (event.peer->address.host == (sm->world->Players.at(i))->Peer.address.host)
					{
						delete sm->world->Players[i];
						sm->world->Players.erase(sm->world->Players.begin() + i);
						std::cout << "player:" << i << " disconnected\n";
						break;
					}
				}
				event.peer->data = NULL;
			}
		}
	}
}
void ServerHost::SendCommand(ENetPeer * peer, int Command, int * Args,int argscount)
{
	++PacketCount;
	int * buffer = new int[argscount + 1];
	ENetPacket * packet;
	buffer[0] = Command;
	for (int i = 0; i < argscount; ++i)
	{
		buffer[i + 1] = Args[i];
	}
	packet = enet_packet_create(buffer, argscount + 1, 0);
	enet_peer_send(peer, 0, packet);
	enet_host_flush(server);
	delete buffer;
	//delete []buffer;
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
						sm->world->ObjectArray[Id]->QuedMovePos[i] = event.packet->data[i + 2] / event.packet->data[i + 8];
					}
					if (event.packet->data[i + 5] != 0){
						sm->world->ObjectArray[Id]->QuedMoveRot[i] = event.packet->data[i + 5] / event.packet->data[i + 8];
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
				neV3 pos;pos.Set(sm->world->ObjectArray[o]->PhysicsBody->GetPos());
				neV3 rot;rot.Set(sm->world->ObjectArray[o]->RotationEuler);
				int x, y, z;
				x = (int)pos[0] * 10; y = (int)pos[1] * 10; z = (int)pos[2] * 10;
				int rx, ry, rz;
				rx = (int)rot[0] * 10; ry = (int)rot[1] * 10; rz = (int)rot[2] * 10;
				enet_uint8 * args = new enet_uint8[10];
				args[0] = Sword_Object;
				args[1] = o;
				args[2] = x;
				args[3] = y;
				args[4] = z;
				args[5] = rx;
				args[6] = ry;
				args[7] = rz;
				args[8] = 0;
				args[9] = sm->world->ObjectArray[o]->Mesh;
				ENetPacket * packet = enet_packet_create("", 10,0);
				packet->data = (enet_uint8 *)args;
				enet_host_broadcast(server, 0, packet);
				enet_host_flush(server);
				//SendCommand(&sm->world->Players[i]->Peer, Sword_Object, args, 9);
				//delete [] args;
			}
		}
	}
}