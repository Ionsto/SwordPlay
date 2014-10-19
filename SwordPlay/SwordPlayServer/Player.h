#pragma once
#include <enet\enet.h>
class ServerManager;
class Player
{
public:
	int CameraId;
	int Id;
	ENetPeer * Peer;
	Player();
	~Player();
};

