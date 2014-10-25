#include "World.h"
#include "GameManager.h"

World::World(GameManager * gm)
{
	Player.QuedBodyMovement = vector3df();
	Player.QuedBodyRotation = vector3df();
	for (int i = 0; i < 10; ++i)
		Player.PlayerObjectIds[i] = -1;
	ObjectCount = 100;
	ObjectArray = new Object*[ObjectCount];
	for (int i = 0; i < ObjectCount;++i)
	{
		ObjectArray[i] = NULL;
	}
	MeshCount = 2;
	Meshes = new IAnimatedMesh*[MeshCount];
	//Lel
	Meshes[0] = gm->smgr->getMesh("./Res/HeadNull.obj"); Meshes[0]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[1] = gm->smgr->getMesh("./Res/Head.obj"); Meshes[1]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[2] = gm->smgr->getMesh("./Res/Arm.obj"); Meshes[2]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[3] = gm->smgr->getMesh("./Res/Leg.obj"); Meshes[2]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[4] = gm->smgr->getMesh("./Res/Body.obj"); Meshes[2]->setMaterialFlag(EMF_LIGHTING, false);
	Meshes[5] = gm->smgr->getMesh("./Res/Sword.obj"); Meshes[2]->setMaterialFlag(EMF_LIGHTING, false);

	Camera = gm->smgr->addCameraSceneNode(0, vector3df(20, 20, 20),vector3df(0,0,0));
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