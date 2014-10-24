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
	//MainMenu = GuiMain();
	//MainMenu.StartDisplay();
	world = new World(this);
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
	const float poooowwwwwerrrr = 12;// !
	if (this->KeyListener.IsKeyDown(KEY_ESCAPE))
	{
		Running = false;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_A))
	{
		world->QuedBodyMovement.X = poooowwwwwerrrr;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_D))
	{
		world->QuedBodyMovement.X = -poooowwwwwerrrr;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_W))
	{
		world->QuedBodyMovement.Z = poooowwwwwerrrr;
	}
	if (this->KeyListener.IsKeyDown(KEY_KEY_S))
	{
		world->QuedBodyMovement.Z = -poooowwwwwerrrr;
	}
	if (this->KeyListener.IsKeyDown(KEY_SPACE))
	{
		world->QuedBodyMovement.Y = 100;
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
	if (this->KeyListener.MouseState.PosChanged)
	{
		float x = KeyListener.MouseState.PrevPosition.X - KeyListener.MouseState.Position.X;
		float y = KeyListener.MouseState.PrevPosition.Y - KeyListener.MouseState.Position.Y;
		float speed = 1;
		if (x > speed)
			x = speed;
		if (y > speed)
			y = speed;
		world->QuedBodyRotation.set(world->QuedBodyRotation.X + y, world->QuedBodyRotation.Y + x, world->QuedBodyRotation.Z);
		KeyListener.MouseState.PrevPosition = KeyListener.MouseState.Position;
		KeyListener.MouseState.PosChanged = false;
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
			enet_uint8 * Args = new enet_uint8[13];
			Args[0] = world->PlayerObjectIds[world->QuedMovment[i].Id];
			Args[1] = (int)(world->QuedMovment[i].dPos.X) / (10 * Connector->GetExp((int)world->QuedMovment[i].dPos.X));
			Args[2] = (int)(world->QuedMovment[i].dPos.Y) / (10 * Connector->GetExp((int)world->QuedMovment[i].dPos.Y));
			Args[3] = (int)(world->QuedMovment[i].dPos.Z) / (10 * Connector->GetExp((int)world->QuedMovment[i].dPos.Z));
			Args[4] = (int)(world->QuedMovment[i].dRot.X) / (10 * Connector->GetExp((int)world->QuedMovment[i].dRot.X));
			Args[5] = (int)(world->QuedMovment[i].dRot.Y) / (10 * Connector->GetExp((int)world->QuedMovment[i].dRot.Y));
			Args[6] = (int)(world->QuedMovment[i].dRot.Z) / (10 * Connector->GetExp((int)world->QuedMovment[i].dPos.Z));
			
			Args[7] = Connector->GetExp((int)world->QuedMovment[i].dPos.X);
			Args[8] = Connector->GetExp((int)world->QuedMovment[i].dPos.Y);
			Args[9] = Connector->GetExp((int)world->QuedMovment[i].dPos.Z);
			Args[10] = Connector->GetExp((int)world->QuedMovment[i].dRot.X);
			Args[11] = Connector->GetExp((int)world->QuedMovment[i].dRot.Y);
			Args[12] = Connector->GetExp((int)world->QuedMovment[i].dPos.Z);
			Connector->SendCommands(Sword_MoveObject, Args, 13);
			delete Args;
		}
		world->QuedMovment.clear();
	}
	if (world->QuedBodyMovement.X != 0 || world->QuedBodyMovement.Y != 0 || world->QuedBodyMovement.Z != 0 || world->QuedBodyRotation.X != 0 || world->QuedBodyRotation.Y != 0 || world->QuedBodyRotation.Z != 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (world->PlayerObjectIds[i] != -1)
			{
				enet_uint8 * Args = new enet_uint8[13];
 				Args[0] = world->PlayerObjectIds[world->PlayerObjectIds[i]];
				Args[1] = ((int)world->QuedBodyMovement.X) / (10 * Connector->GetExp((int)world->QuedBodyMovement.X));
				Args[2] = ((int)world->QuedBodyMovement.Y) / (10 * Connector->GetExp((int)world->QuedBodyMovement.Y));
				Args[3] = ((int)world->QuedBodyMovement.Z) / (10 * Connector->GetExp((int)world->QuedBodyMovement.Z));
				Args[4] = ((int)world->QuedBodyRotation.X) / (10 * Connector->GetExp((int)world->QuedBodyRotation.X));
				Args[5] = ((int)world->QuedBodyRotation.Y) / (10 * Connector->GetExp((int)world->QuedBodyRotation.Y));
				Args[6] = ((int)world->QuedBodyRotation.Z) / (10 * Connector->GetExp((int)world->QuedBodyRotation.Z));

				Args[7] = Connector->GetExp(world->QuedBodyMovement.X);
				Args[8] = Connector->GetExp(world->QuedBodyMovement.Y);
				Args[9] = Connector->GetExp(world->QuedBodyMovement.Z);
				Args[10] = Connector->GetExp(world->QuedBodyRotation.X);
				Args[11] = Connector->GetExp(world->QuedBodyRotation.X);
				Args[12] = Connector->GetExp(world->QuedBodyRotation.X);
				Connector->SendCommands(Sword_MoveObject, Args, 13);
				delete[] Args;
			}
		}
		world->QuedBodyMovement = vector3df(0,0,0);
		world->QuedBodyRotation = vector3df(0,0,0);
	}
	Connector->GetInfomation(this);

	if (world->PlayerObjectIds[Sword_PlayerId_Head] != -1)
	{
		if (world->ObjectArray[world->PlayerObjectIds[Sword_PlayerId_Head]] != NULL)
		{
			//world->MoveCamera(world->ObjectArray[world->PlayerObjectIds[Sword_PlayerId_Head]]->Node->getAbsolutePosition(), world->ObjectArray[world->PlayerObjectIds[Sword_PlayerId_Head]]->Node->getAbsoluteTransformation().getRotationDegrees());
		}
		else
		{
			world->PlayerObjectIds[Sword_PlayerId_Head] = -1;
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