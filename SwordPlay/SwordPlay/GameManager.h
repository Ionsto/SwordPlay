#pragma once
#include <Irrlicht\irrlicht.h>
#include "KeyListener.h"
#include "GuiMain.h"
#include "World.h"
#include "ClientConnection.h"
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class GameManager
{
public:
	bool Running;
	IrrlichtDevice *device;
	ISceneManager * smgr;
	IVideoDriver* driver;
	IGUIEnvironment* guienv;
	KeyListener KeyListener;
	GuiMain MainMenu;
	World * world;
	ClientConnection * Connector;
	GameManager();
	~GameManager();
	void Run();
	void MainLoop();
	void Init();
	void Destroy();
	void Render();
	void Update();
	int InitIrrlicht();
	void UpdateServer();
};

