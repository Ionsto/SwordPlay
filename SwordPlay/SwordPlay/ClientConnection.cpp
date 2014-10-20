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

void ClientConnection::SendCommands(int Command, int * Args,int argscount)
{
	ENetPacket *packet;
	int * buffer = new int[argscount + 1];
	buffer[0] = Command;
	for (int i = 0; i < argscount; ++i)
	{
		buffer[i + 1] = Args[i];
	}
	packet = enet_packet_create(buffer, argscount + 1, 0);
	enet_peer_send(Server, 0, packet);
}

void ClientConnection::GetInfomation(GameManager * gm)
{
	ENetEvent event;
	std::vector<ENetEvent> Recivevents;
	while (enet_host_service(client, &event, 0) > 0)
	{
	switch (event.type) {
		case ENET_EVENT_TYPE_RECEIVE:
			Recivevents.push_back(event);
			//puts((char*)event.packet->data);
			break;
		case ENET_EVENT_TYPE_DISCONNECT:
			printf("You have been disconnected.\n");
			enet_packet_destroy(event.packet);
			break;
		}
	}
	for (int i = 0; i < Recivevents.size();++i)
	{
		ParsePacket(gm,Recivevents[i]);
		enet_packet_destroy(Recivevents[i].packet);
	}
}
void ClientConnection::ParsePacket(GameManager * gm, ENetEvent event)
{
	if (((int)event.packet->data[0]) == Sword_Object)
	{
		int id = event.packet->data[1];
		int X = event.packet->data[2] / 10;
		int Y = event.packet->data[3] / 10;
		int Z = event.packet->data[4] / 10;
		int RX = event.packet->data[5] / 10;
		int RY = event.packet->data[6] / 10;
		int RZ = event.packet->data[7] / 10;
		if (gm->world->ObjectArray[id] == NULL)
		{
			int Mesh = (int)event.packet->data[8];
			if (Mesh < 0){ Mesh = 0; }
			if (Mesh > gm->world->MeshCount){ Mesh = gm->world->MeshCount - 1; }
			if (gm->world->PlayerObjectIds[Sword_PlayerId_Head] != -1 && gm->world->PlayerObjectIds[Sword_PlayerId_Head] == id)
			{
				//The head of the mesh family
				Mesh = 0;
			}
			gm->world->ObjectArray[id] = new Object(gm, Mesh);
		}
		std::cout << X << ":" << Y << ":" << Z << "\n";
		gm->world->ObjectArray[id]->Node->setPosition(vector3df(X, Y, Z));
		gm->world->ObjectArray[id]->Node->setRotation(vector3df(RX, RY, RZ));
		if (gm->world->PlayerObjectIds[Sword_PlayerId_Head] != -1)
		{
			//gm->world->MoveCamera(vector3df(X, Y, Z), vector3df(RX, RY, RZ));
		}
	}
	if (((int)event.packet->data[0]) == Sword_PlayerIds)
	{
		gm->world->PlayerObjectIds[event.packet->data[1]] = (int)event.packet->data[2];
	}
}