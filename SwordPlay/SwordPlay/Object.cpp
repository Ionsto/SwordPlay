#include "Object.h"
#include "GameManager.h"

Object::Object(GameManager * gm,int mesh)
{
	//Type = 0;
	LOD = 0;
	PlayerId = -1;
	Mesh = mesh;
	Node = gm->smgr->addMeshSceneNode(gm->world->Meshes[Mesh]);
	Node->setMaterialTexture(0, gm->driver->getTexture("./Res/Arm.png"));
	Node->setPosition(vector3df(0,0,0));
	Node->setRotation(vector3df(90, 0, 0));
}


Object::~Object()
{
}
void Object::Update(GameManager * gm)
{
}