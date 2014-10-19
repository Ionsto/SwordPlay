#include "Player.h"
#include "ServerManager.h"

Player::Player()
{
	for(int i = 0;i < 10;++i)
		BodyPartIds[i] = -1;
}


Player::~Player()
{
	//delete Peer;
}
