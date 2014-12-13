#include "ClientConnection.h"
#include <enet\enet.h>
#include <iostream>
#include "GameManager.h"
#include <vector>

ClientConnection::ClientConnection()
{
	client = NULL;
	client = enet_host_create(NULL /* create a client host */,
		1 /* only allow 1 outgoing connection */,
		2 /* allow up 2 channels to be used, 0 and 1 */,
		57600 / 8 /* 56K modem with 56 Kbps downstream bandwidth */,
		14400 / 8 /* 56K modem with 14 Kbps upstream bandwidth */);
	if (client == NULL)
	{
		std::cout << "An error occurred while trying to create an ENet client host.\n";
		exit(EXIT_FAILURE);
	}
}


ClientConnection::~ClientConnection()
{
	if (client != NULL)
	{
		enet_host_destroy(client);
	}
}
void ClientConnection::Connect(char * ip,int port)
{
	/* Connect to some.server.net:1234. */
	enet_address_set_host(&address, ip);
	address.port = port;
	/* Initiate the connection, allocating the two channels 0 and 1. */
	Server = enet_host_connect(client, &address, 2, 0);
	if (Server == NULL)
	{
		std::cout<<"No available peers for initiating an ENet connection.\n";
		//exit(EXIT_FAILURE);
	}
	/* Wait up to 5 seconds for the connection attempt to succeed. */
	if (enet_host_service(client, &event, 5000) > 0 &&
		event.type == ENET_EVENT_TYPE_CONNECT)
	{
		puts("Connection to server succeeded.");
	}
	else
	{
		/* Either the 5 seconds are up or a disconnect event was */
		/* received. Reset the peer in the event the 5 seconds */
		/* had run out without any significant event. */
		enet_peer_reset(Server);
		puts("Connection to some.server.net:1234 failed.");
	}
}
void ClientConnection::Disconnect()
{
	enet_peer_disconnect(Server, 0);
	/* Allow up to 3 seconds for the disconnect to succeed
	* and drop any packets received packets.
	*/
	while (enet_host_service(client, &event, 3000) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_RECEIVE:
			enet_packet_destroy(event.packet);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			puts("Disconnection succeeded.");
			return;
		}
	}
	/* We've arrived here, so the disconnect attempt didn't */
	/* succeed yet. Force the connection down. */
	enet_peer_reset(Server);
}

void ClientConnection::SendCommands(int Command, enet_uint8 * Args, int argscount)
{
	ENetPacket *packet;
	enet_uint8 * buffer = new enet_uint8[argscount + 1];
	buffer[0] = Command;
	for (int i = 0; i < argscount; ++i)
	{
		buffer[i + 1] = Args[i];
	}
	packet = enet_packet_create(NULL, argscount + 1, 0);
	packet->data = buffer;
	enet_peer_send(Server, 0, packet);
	enet_host_flush(client);
}

void ClientConnection::GetInfomation(GameManager * gm)
{
	ENetEvent event;
	int Count = 0;
	while (enet_host_service(client, &event, 0) > 0 && Count++ < 100)
	{
	switch (event.type) {
	case ENET_EVENT_TYPE_RECEIVE:
			ParsePacket(gm, event);
			enet_packet_destroy(event.packet);
			//puts((char*)event.packet->data);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("You have been disconnected.\n");
			enet_packet_destroy(event.packet);
			break;
		}
	}
}
void ClientConnection::ParsePacket(GameManager * gm, ENetEvent event)
{
	if (((int)event.packet->data[0]) == Sword_Object)
	{
		int id = ((INT16)event.packet->data[1]);
		UINT8 X0 = (event.packet->data[2]);
		UINT8 X1 = (event.packet->data[3]);
		INT16 X = ((X0 << 8) | X1);
		UINT8 Y0 = (event.packet->data[4]);
		UINT8 Y1 = (event.packet->data[5]);
		INT16 Y = ((Y0 << 8) | Y1);
		UINT8 Z0 = (event.packet->data[6]);
		UINT8 Z1 = (event.packet->data[7]);
		INT16 Z = ((Z0 << 8) | Z1);
		UINT8 RX0 = (event.packet->data[8]);
		UINT8 RX1 = (event.packet->data[9]);
		INT16 RX = ((RX0 << 8) | RX1);
		UINT8 RY0 = (event.packet->data[10]);
		UINT8 RY1 = (event.packet->data[11]);
		INT16 RY = ((RY0 << 8) | RY1);
		UINT8 RZ0 = (event.packet->data[12]);
		UINT8 RZ1 = (event.packet->data[13]);
		INT16 RZ = ((RZ0 << 8) | RZ1);
		if (gm->world->ObjectArray[id] == NULL)
		{
			int Mesh = (int)event.packet->data[15];
			Mesh = 1;
			if (Mesh < 0){ Mesh = 0; }
			if (Mesh > gm->world->MeshCount){ Mesh = gm->world->MeshCount - 1; }
			if (gm->world->Player.PlayerObjectIds[Sword_PlayerId_Head] != -1 && gm->world->Player.PlayerObjectIds[Sword_PlayerId_Head] == id)
			{
				//The head of the mesh family
				Mesh = 1;
			}
			gm->world->ObjectArray[id] = new Object(gm, Mesh);
			if (gm->world->Player.PlayerObjectIds[Sword_PlayerId_Head] != -1 && gm->world->Player.PlayerObjectIds[Sword_PlayerId_Head] == id)
			{
				//gm->world->Camera->setParent(gm->world->ObjectArray[id]->Node);
				//gm->world->MoveCamera(vector3df(0, 0 + 10, 0), vector3df(0, 0, 0));
				//gm->world->Camera->getAbsolutePosition();
				//gm->world->MoveCamera(vector3df(0, 0 + 10, 0), vector3df(0, 0, 0));
				//gm->world->Camera->setTarget(vector3df(0, 0, 0));
			}
		}
		//std::cout << ",RX:" << RX << ",RY:" << RY << ",RZ:" << RZ << "\n";
		gm->world->ObjectArray[id]->Node->setRotation(vector3df(RX, RY,RZ));
		gm->world->ObjectArray[id]->Node->setPosition(vector3df(X, Y, Z));
		if (gm->world->Player.PlayerObjectIds[Sword_PlayerId_Head] != -1)
		{
			//gm->world->MoveCamera(vector3df(X, Y + 5, Z), vector3df(RX, RY, RZ));
			//Camera->updateAbsolutePosition();
		}
	}
	if (((int)event.packet->data[0]) == Sword_PlayerIds)
	{
		enet_uint8 bodyid = event.packet->data[1];
		enet_uint8 worldid = event.packet->data[2];
		gm->world->Player.PlayerObjectIds[(int)bodyid] = (INT8)worldid;
	}
}