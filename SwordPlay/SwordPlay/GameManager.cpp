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
	if (world->Player.PlayerObjectIds[Sword_PlayerId_Head] != -1)
	{
		if (world->ObjectArray[world->Player.PlayerObjectIds[Sword_PlayerId_Head]] != NULL)
		{
			float theta = world->ObjectArray[world->Player.PlayerObjectIds[0]]->Node->getRotation().Y;
			//std::cout << "X:" << world->ObjectArray[world->PlayerObjectIds[0]]->Node->getRotation().X << ",Y:" << world->ObjectArray[world->PlayerObjectIds[0]]->Node->getRotation().Y << ",Z:" << world->ObjectArray[world->PlayerObjectIds[0]]->Node->getRotation().Z << "\n";
			if (theta < 0){ theta += 360; }
			if (theta > 360){ theta -= 360; }
			if (this->KeyListener.IsKeyDown(KEY_KEY_A))
			{
				world->Player.QuedBodyMovement.X = poooowwwwwerrrr;
			}
			if (this->KeyListener.IsKeyDown(KEY_KEY_D))
			{
				world->Player.QuedBodyMovement.X = -poooowwwwwerrrr;
			}
			if (this->KeyListener.IsKeyDown(KEY_KEY_W))
			{
				world->Player.QuedBodyMovement.X = poooowwwwwerrrr * cosf(theta * (3.14 / 180.0));
				world->Player.QuedBodyMovement.Z = poooowwwwwerrrr * sinf(theta * (3.14 / 180.0));
			}
			if (this->KeyListener.IsKeyDown(KEY_KEY_S))
			{
				world->Player.QuedBodyMovement.X = -poooowwwwwerrrr * cosf(theta * (3.14 / 180.0));
				world->Player.QuedBodyMovement.Z = -poooowwwwwerrrr * sinf(theta * (3.14 / 180.0));
			}
			if (this->KeyListener.IsKeyDown(KEY_SPACE))
			{
				//world->QuedBodyMovement.Y = 50;
			}
			float rotpowwwwweeerr = 10;
			if (this->KeyListener.IsKeyDown(KEY_KEY_Q))
			{
				world->Player.QuedBodyRotation.Y = rotpowwwwweeerr;
				//world->MoveCamera(world->Camera->getPosition() + vector3df(0, 0.1, 0));
				//world->Camera->updateAbsolutePosition();
				//world->Camera->setRotation(vector3df(50, 0, 0));
			}
			if (this->KeyListener.IsKeyDown(KEY_KEY_E))
			{
				world->Player.QuedBodyRotation.Y = -rotpowwwwweeerr;
				//world->MoveCamera(world->Camera->getPosition() + vector3df(0, -0.1, 0));
				//world->Camera->updateAbsolutePosition();
				//world->Camera->setRotation(vector3df(50, 0, 0));
			}
			if (this->KeyListener.MouseState.PosChanged)
			{
				float x = KeyListener.MouseState.PrevPosition.X - KeyListener.MouseState.Position.X;
				float y = KeyListener.MouseState.PrevPosition.Y - KeyListener.MouseState.Position.Y;
				float speed = 1;
				if (abs(x) > speed)
					x = speed * (x / abs(x));
				if (abs(y) > speed)
					y = speed * (y / abs(y));
				world->Player.QuedBodyRotation.set(world->Player.QuedBodyRotation.X + y, world->Player.QuedBodyRotation.Y + x, world->Player.QuedBodyRotation.Z);
				KeyListener.MouseState.PrevPosition = KeyListener.MouseState.Position;
				KeyListener.MouseState.PosChanged = false;
			}
		}
	}
	world->Update(this);
}
void GameManager::UpdateServer()
{
	//int args[5] = {0,0,0,0};
	//Connector->SendCommands(RTS_MOVE_UNIT, args);
	if (world->Player.QuedMovment.size() != 0)
	{
		for (int i = 0; i < world->Player.QuedMovment.size(); ++i)
		{
			enet_uint8 * Args = new enet_uint8[13];
			Args[0] = world->Player.PlayerObjectIds[world->Player.QuedMovment[i].Id];
			Args[1] = (int)(world->Player.QuedMovment[i].dPos.X) / (10 * Connector->GetExp((int)world->Player.QuedMovment[i].dPos.X));
			Args[2] = (int)(world->Player.QuedMovment[i].dPos.Y) / (10 * Connector->GetExp((int)world->Player.QuedMovment[i].dPos.Y));
			Args[3] = (int)(world->Player.QuedMovment[i].dPos.Z) / (10 * Connector->GetExp((int)world->Player.QuedMovment[i].dPos.Z));
			Args[4] = (int)(world->Player.QuedMovment[i].dRot.X) / (10 * Connector->GetExp((int)world->Player.QuedMovment[i].dRot.X));
			Args[5] = (int)(world->Player.QuedMovment[i].dRot.Y) / (10 * Connector->GetExp((int)world->Player.QuedMovment[i].dRot.Y));
			Args[6] = (int)(world->Player.QuedMovment[i].dRot.Z) / (10 * Connector->GetExp((int)world->Player.QuedMovment[i].dPos.Z));
			
			Args[7] = Connector->GetExp((int)world->Player.QuedMovment[i].dPos.X);
			Args[8] = Connector->GetExp((int)world->Player.QuedMovment[i].dPos.Y);
			Args[9] = Connector->GetExp((int)world->Player.QuedMovment[i].dPos.Z);
			Args[10] = Connector->GetExp((int)world->Player.QuedMovment[i].dRot.X);
			Args[11] = Connector->GetExp((int)world->Player.QuedMovment[i].dRot.Y);
			Args[12] = Connector->GetExp((int)world->Player.QuedMovment[i].dPos.Z);
			Connector->SendCommands(Sword_MoveObject, Args, 13);
			delete Args;
		}
		world->Player.QuedMovment.clear();
	}
	if (world->Player.QuedBodyMovement.X != 0 || world->Player.QuedBodyMovement.Y != 0 || world->Player.QuedBodyMovement.Z != 0 || world->Player.QuedBodyRotation.X != 0 || world->Player.QuedBodyRotation.Y != 0 || world->Player.QuedBodyRotation.Z != 0)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (world->Player.PlayerObjectIds[i] != -1)
			{
				enet_uint8 * Args = new enet_uint8[13];
				Args[0] = world->Player.PlayerObjectIds[world->Player.PlayerObjectIds[i]];
				Args[1] = ((int)world->Player.QuedBodyMovement.X) / (10 * Connector->GetExp((int)world->Player.QuedBodyMovement.X));
				Args[2] = ((int)world->Player.QuedBodyMovement.Y) / (10 * Connector->GetExp((int)world->Player.QuedBodyMovement.Y));
				Args[3] = ((int)world->Player.QuedBodyMovement.Z) / (10 * Connector->GetExp((int)world->Player.QuedBodyMovement.Z));
				Args[4] = ((int)world->Player.QuedBodyRotation.X) / (10 * Connector->GetExp((int)world->Player.QuedBodyRotation.X));
				Args[5] = ((int)world->Player.QuedBodyRotation.Y) / (10 * Connector->GetExp((int)world->Player.QuedBodyRotation.Y));
				Args[6] = ((int)world->Player.QuedBodyRotation.Z) / (10 * Connector->GetExp((int)world->Player.QuedBodyRotation.Z));

				Args[7] = Connector->GetExp(world->Player.QuedBodyMovement.X);
				Args[8] = Connector->GetExp(world->Player.QuedBodyMovement.Y);
				Args[9] = Connector->GetExp(world->Player.QuedBodyMovement.Z);
				Args[10] = Connector->GetExp(world->Player.QuedBodyRotation.X);
				Args[11] = Connector->GetExp(world->Player.QuedBodyRotation.Y);
				Args[12] = Connector->GetExp(world->Player.QuedBodyRotation.Z);
				Connector->SendCommands(Sword_MoveObject, Args, 13);
				delete[] Args;
			}
		}
		world->Player.QuedBodyMovement = vector3df(0, 0, 0);
		world->Player.QuedBodyRotation = vector3df(0, 0, 0);
	}
	Connector->GetInfomation(this);

	if (world->Player.PlayerObjectIds[Sword_PlayerId_Head] != -1)
	{
		if (world->ObjectArray[world->Player.PlayerObjectIds[Sword_PlayerId_Head]] != NULL)
		{
			//world->MoveCamera(world->ObjectArray[world->PlayerObjectIds[Sword_PlayerId_Head]]->Node->getAbsolutePosition(), world->ObjectArray[world->PlayerObjectIds[Sword_PlayerId_Head]]->Node->getAbsoluteTransformation().getRotationDegrees());
		}
		else
		{
			//world->Player.PlayerObjectIds[Sword_PlayerId_Head] = -1;
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