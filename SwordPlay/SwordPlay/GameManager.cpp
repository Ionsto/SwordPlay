#include "GameManager.h"
#include "World.h"
#include <enet\enet.h>
GameManager::GameManager()
{
	Running = true;
}


GameManager::~GameManager()
{
}

void GameManager::Run()
{
	Init();
	MainLoop();
	Destroy();
}
void GameManager::Init()
{
	if (InitIrrlicht() != 0){ return; }
	MainMenu = GuiMain();
	MainMenu.StartDisplay();
	world = new World(this);
	//world->AddObject(new Object(this), true);
	enet_initialize();
	Connector = new ClientConnection();
	Connector->Connect("localhost", 25565);
}
int GameManager::InitIrrlicht()
{
	//Null device
	device = createDevice(video::EDT_NULL);
	dimension2d<u32> dim = device->getVideoModeList()->getDesktopResolution();
	dim = dimension2d<u32>(800, 800);
	device = createDevice(video::EDT_OPENGL, dim, 16, false, false, false, &KeyListener);
	if (!device){ return 1; }
	driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	guienv = device->getGUIEnvironment();
	return 0;
}
void GameManager::MainLoop()
{
	while (Running)
	{
		Update();
		UpdateServer();
		Render();
		if (!device->run()){ Running = false; }
	}
	this->Connector->Disconnect();
}
void GameManager::Update()
{
	MainMenu.Update(this);
	if (this->KeyListener.IsKeyDown(KEY_ESCAPE))
	{
		Running = false;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_A))
	{
		world->QuedBodyMovement.X = 1;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_D))
	{
		world->QuedBodyMovement.X = -1;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_W))
	{
		world->QuedBodyMovement.Z = 1;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_S))
	{
		world->QuedBodyMovement.Z = -1;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_Q))
	{
		//world->MoveCamera(world->Camera->getPosition() + vector3df(0, 0.1, 0));
		//world->Camera->updateAbsolutePosition();
		//world->Camera->setRotation(vector3df(50, 0, 0));
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_E))
	{
		//world->MoveCamera(world->Camera->getPosition() + vector3df(0, -0.1, 0));
		//world->Camera->updateAbsolutePosition();
		//world->Camera->setRotation(vector3df(50, 0, 0));
	}
	world->Update(this);
}
void GameManager::UpdateServer()
{
	//int args[5] = {0,0,0,0};
	//Connector->SendCommands(RTS_MOVE_UNIT, args);
	if (world->QuedMovment.size() != 0)
	{
		for (int i = 0; i < world->QuedMovment.size(); ++i)
		{
			Connector->SendCommands(Sword_MovePlayer, {}, 6);
		}
	}
	Connector->SendCommands(Sword_MovePlayer, new int [] { (int)world->QuedBodyMovement.X, (int)world->QuedBodyMovement.Y, (int)world->QuedBodyMovement.Z, (int)world->QuedBodyRotation.X, (int)world->QuedBodyRotation.Y, (int)world->QuedBodyRotation.Z }, 6);
	world->QuedBodyMovement = vector3df();
	world->QuedBodyRotation = vector3df();
	Connector->GetInfomation(this);

	if (world->CameraObjectId != -1)
	{
		if (world->ObjectArray[world->CameraObjectId] != NULL)
		{
			world->MoveCamera(world->ObjectArray[world->CameraObjectId]->Node->getAbsolutePosition(), world->ObjectArray[world->CameraObjectId]->Node->getAbsoluteTransformation().getRotationDegrees());
		}
		else
		{
			world->CameraObjectId = -1;
		}
	}
}
void GameManager::Destroy()
{
	this->device->drop();
	enet_deinitialize();
	delete Connector;
	delete world;
}
void GameManager::Render()
{
	driver->beginScene(true, true, SColor(255, 100, 101, 140));
	smgr->drawAll();
	guienv->drawAll();
	driver->endScene();
}