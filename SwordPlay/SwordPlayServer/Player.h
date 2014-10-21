#pragma once
#include <enet\enet.h>
class ServerManager;
class Player
{
public:
	/*
	Head = 0;
	Body = 1;
	ArmUpperR = 2;
	ArmUpperL = 3;
	ArmLowerR = 4;
	ArmLowerL = 5;
	LegUpperR = 6;
	LegUpperL = 7;
	LegLowerR = 8;
	LegLowerL = 9;
	*/
	int * BodyPartIds;
	int Id;
	ENetPeer Peer;
	Player();
	~Player();
};

