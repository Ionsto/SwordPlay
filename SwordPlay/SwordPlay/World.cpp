#include "World.h"
#include "GameManager.h"

World::World(GameManager * gm)
{
	QuedBodyMovement = vector3df();
	QuedBodyRotation = vector3df();
	CameraObjectId = -1;
	ObjectCount = 100;
	ObjectArray = new Object*[ObjectCount];
	for (int i = 0; i < ObjectCount;++i)
	{
		ObjectArray[i] = NULL;
	}
	MeshCount = 1;
	Meshes = new IAnimatedMesh*[MeshCount];
	Meshes[0] = gm->smgr->getMesh("./Res/Player.x");
	Meshes[0]->setMaterialFlag(EMF_LIGHTING, false);
	Camera = gm->smgr->addCameraSceneNode(0, vector3df(0, 20, 0));
	Camera->bindTargetAndRotation(true);
	Camera->updateAbsolutePosition();
}
World::~World()
{
	for (int i = 0; i < ObjectCount;++i)
	{
		if (ObjectArray[i] != NULL)
		{
			delete ObjectArray[i];
		}
	}
	delete ObjectArray;
}
void World::Update(GameManager * gm)
{
}
void World::MoveCamera(vector3df pos,vector3df rot)
{
	Camera->setPosition(pos);
	Camera->setRotation(rot);
}