#include "World.h"
#include "ServerManager.h"


World::World()
{
	neSimulatorSizeInfo sizeInfo;
	RigidBodyCount = 40;
	ObjectCount = 20;
	sizeInfo.rigidBodiesCount = this->RigidBodyCount;
	sizeInfo.animatedBodiesCount = 0;
	neV3 gravity = neV3();
	gravity.Set(0.0f, -10.0f, 0.0f);
	s32 totalBody = sizeInfo.rigidBodiesCount + sizeInfo.animatedBodiesCount;
	sizeInfo.geometriesCount = totalBody;
	//see sdk for this formula:
	sizeInfo.overlappedPairsCount = totalBody * (totalBody - 1) / 2;
	sizeInfo.rigidParticleCount = 0;

	//all information gathered. So create the simulator:
	m_Sim = neSimulator::CreateSimulator(sizeInfo, NULL, &gravity);
	ObjectArray = new Object*[ObjectCount];
	for (int i = 0; i < ObjectCount; ++i)
	{
		ObjectArray[i] = NULL;
	}
	ObjectArray[AddObject(new Object(this))]->Init(this, 4, 0, 0);
	ObjectArray[AddObject(new Object(this))]->Init(this, 9, 0, 3);
	ObjectArray[AddObject(new Object(this))]->Init(this, 0, 0, 3);
	//ObjectArray[AddObject(new Object(this))]->Init(this, 0, 10, 0);
}


World::~World()
{
	for (int i = 0; i < ObjectCount; ++i)
	{
		if (ObjectArray[i] != NULL)
		{
			delete ObjectArray[i];
		}
	}
	delete ObjectArray;
	neSimulator::DestroySimulator(m_Sim);
}

int World::AddObject(Object* object, bool candelete,bool caninit)
{
	for (int i = 0; i < ObjectCount; ++i)
	{
		if (ObjectArray[i] == NULL)
		{
			object->Id = i;
			if (caninit){ object->Init(this); }
			ObjectArray[i] = object;
			return i;
		}
	}
	if (candelete)
	{
		delete object;
	}
	return -1;
}
void World::Update(ServerManager * sm)
{
	m_Sim->Advance(0.1);
	for (int i = 0; i < this->ObjectCount; ++i)
	{
		if (ObjectArray[i] != NULL)
		{
			ObjectArray[i]->Update(this);
		}
	}
}
void World::SpawnPlayer(int playerid)
{
	Players[playerid].BodyPartIds[Sword_PlayerId_Head] = AddObject(new Object(this));
	ObjectArray[Players[playerid].BodyPartIds[Sword_PlayerId_Head]]->Init(this, 0, 2, 0);
	ObjectArray[Players[playerid].BodyPartIds[Sword_PlayerId_Head]]->Mesh = 1;
	Players[playerid].BodyPartIds[Sword_PlayerId_Head] = AddObject(new Object(this));
	ObjectArray[Players[playerid].BodyPartIds[Sword_PlayerId_Head]]->Init(this, 0, 0, 0);
	ObjectArray[Players[playerid].BodyPartIds[Sword_PlayerId_Head]]->Mesh = 1;
	//Players[playerid].BodyPartIds[Sword_PlayerId_Body] = AddObject(new Object(this));
}