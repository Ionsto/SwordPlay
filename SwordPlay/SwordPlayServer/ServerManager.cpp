#include "ServerManager.h"
#include <iostream>

ServerManager::ServerManager()
{
	Running = true;
	ClientCount = 0;
	PacketsPerUpdateCounter = 0;
}


ServerManager::~ServerManager()
{
}
void ServerManager::Init()
{
	std::cout << "Initilising\n";
	enet_initialize();
	std::cout << "Genn'ing world\n";
	world = new World();
	/*world->GenerateWorld();
	world->AddObject(new Object());
	world->ObjectArray[0]->Position = Vector2(4, 4);
	world->AddObject(new Object());
	world->ObjectArray[1]->Position = Vector2(6, 4);
	world->AddObject(new Object());
	world->ObjectArray[2]->Position = Vector2(8, 4);*/
	std::cout << "Starting Server\n";
	Serverhost = new ServerHost();
	Serverhost->Start(25565);
}
void ServerManager::Destroy()
{
	delete world;
	delete Serverhost;
	enet_deinitialize();
}
void ServerManager::Run()
{
	Init();
	MainLoop();
	Destroy();
}

void ServerManager::Update()
{
	world->Update(this);
}
void ServerManager::SendClientInfo()
{
	for (int i = 0; i < ClientCount;++i)
	{
		//example
		//std::thread first(foo);

	}
}

void ServerManager::MainLoop()
{
	while (Running)
	{
		Update();
		if (PacketsPerUpdateCounter++ >= PacketsPerUpdate){
			this->Serverhost->UpdateAll(this);
			this->Serverhost->GetMessages(this);
			PacketsPerUpdateCounter = 0;
		}
	}
}