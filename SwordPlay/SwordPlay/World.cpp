#include "World.h"
#include "GameManager.h"

World::World(GameManager * gm)
{
	QuedBodyMovement = vector3df();
	QuedBodyRotation = vector3df();
	for (int i = 0; i < 10; ++i)
		PlayerObjectIds[i] = -1;
	ObjectCount = 100;
	ObjectArray = new Object*[ObjectCount];
	for (int i = 0; i < ObjectCount;++i)
	{
		ObjectArray[i] = NULL;
	}
	MeshCount = 2;
	Meshes = new IAnimatedMesh*[MeshCount];
	//Lel
	Meshes[0] = gm->smgr->getMesh("./Res/HeadNull.x"); Meshes[0]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[1] = gm->smgr->getMesh("./Res/Head.x"); Meshes[1]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[2] = gm->smgr->getMesh("./Res/Arm.x"); Meshes[2]->setMaterialFlag(EMF_LIGHTING, false);

	Camera = gm->smgr->addCameraSceneNode(0, vector3df(30, 30, 30),vector3df(0,0,0));
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