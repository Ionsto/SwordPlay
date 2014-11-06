#pragma once
#include <Irrlicht\irrlicht.h>
#include <Irrlicht\ISceneNode.h>
#include <Irrlicht\IMeshSceneNode.h>
class GameManager;
using namespace irr;
using namespace scene;
class Object
{
public:
	IMeshSceneNode * Node;//Location
	int LOD;
	int PlayerId;
	int Mesh;
	//Location
	int WorldPart;
	Object(GameManager * gm,int);
	~Object();
	void Update(GameManager * gm);
};

