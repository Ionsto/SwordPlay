#include "World.h"
#include "ServerManager.h"


World::World()
{
	neSimulatorSizeInfo sizeInfo;
	ObjectCount = 40;
	RigidBodyCount = ObjectCount + 20;
	sizeInfo.rigidBodiesCount = this->RigidBodyCount;
	sizeInfo.animatedBodiesCount = 5;
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
	neAnimatedBody * gFloor = m_Sim->CreateAnimatedBody();
	// Add geometry to the floor and set it to be a box with size as defined by the FLOORSIZE constant 
	float FLOORSIZE = 100;
	neGeometry * geom = gFloor->AddGeometry();
	neV3 boxSize1; boxSize1.Set(FLOORSIZE, 0.2f, FLOORSIZE);
	geom->SetBoxSize(boxSize1[0],boxSize1[1],boxSize1[2]);
	gFloor->UpdateBoundingInfo(); // Set the position of the box within the simulator 
	neV3 pos; pos.Set(0.0f, -3.0f, 0.0f); gFloor->SetPos(pos);

	neAnimatedBody * Left = m_Sim->CreateAnimatedBody();
	neGeometry * geomL = Left->AddGeometry();
	boxSize1; boxSize1.Set(10, 100, 100);
	geomL->SetBoxSize(boxSize1[0], boxSize1[1], boxSize1[2]);
	Left->UpdateBoundingInfo(); // Set the position of the box within the simulator 
	pos; pos.Set(FLOORSIZE, 50, 0.0f); Left->SetPos(pos);

	neAnimatedBody * Right = m_Sim->CreateAnimatedBody();
	neGeometry * geomR = Right->AddGeometry();
	boxSize1; boxSize1.Set(10, 100, 100);
	geomR->SetBoxSize(boxSize1[0], boxSize1[1], boxSize1[2]);
	Right->UpdateBoundingInfo(); // Set the position of the box within the simulator 
	pos; pos.Set(-FLOORSIZE, 50, 0.0f); Right->SetPos(pos);

	neAnimatedBody * Top = m_Sim->CreateAnimatedBody();
	neGeometry * geomT = Top->AddGeometry();
	boxSize1; boxSize1.Set(100, 100, 10);
	geomT->SetBoxSize(boxSize1[0], boxSize1[1], boxSize1[2]);
	Top->UpdateBoundingInfo(); // Set the position of the box within the simulator 
	pos; pos.Set(0, 50, FLOORSIZE); Top->SetPos(pos);

	neAnimatedBody * Bottem = m_Sim->CreateAnimatedBody();
	neGeometry * geomB = Bottem->AddGeometry();
	boxSize1; boxSize1.Set(100, 100, 10);
	geomB->SetBoxSize(boxSize1[0], boxSize1[1], boxSize1[2]);
	Bottem->UpdateBoundingInfo(); // Set the position of the box within the simulator 
	pos; pos.Set(0, 50, -FLOORSIZE); Bottem->SetPos(pos);
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
	for (int i = 0; i < Players.size(); ++i)
	{
		delete Players[i];
	}
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
const float DT = 1.0 / 50.0;
void World::Update(ServerManager * sm)
{
	m_Sim->Advance(DT);
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
	float OffsetX = 0, OffsetY = 0, OffsetZ = 0;
	float LegHeight = 2.4;
	float LegWidth = 4;
	float BodyHeight = 10;
	
	Players[playerid]->BodyPartIds[Sword_PlayerId_LegLowerL] = AddObject(new Object(this));
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegLowerL]]->Init(this, OffsetX + LegWidth, OffsetY, OffsetZ);
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegLowerL]]->Mesh = 3;
	Players[playerid]->BodyPartIds[Sword_PlayerId_LegLowerR] = AddObject(new Object(this));
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegLowerR]]->Init(this, OffsetX - LegWidth, OffsetY, OffsetZ);
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegLowerR]]->Mesh = 3;
	
	Players[playerid]->BodyPartIds[Sword_PlayerId_LegUpperL] = AddObject(new Object(this));
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegUpperL]]->Init(this, OffsetX + LegWidth, OffsetY + LegHeight, OffsetZ);
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegUpperL]]->Mesh = 1;
	Players[playerid]->BodyPartIds[Sword_PlayerId_LegUpperR] = AddObject(new Object(this));
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegUpperR]]->Init(this, OffsetX - LegWidth, OffsetY + LegHeight, OffsetZ);
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_LegUpperR]]->Mesh = 1;
	Players[playerid]->BodyPartIds[Sword_PlayerId_Head] = AddObject(new Object(this));
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_Head]]->Init(this, OffsetX, OffsetY + (LegHeight*2) + BodyHeight, OffsetZ);
	ObjectArray[Players[playerid]->BodyPartIds[Sword_PlayerId_Head]]->Mesh = 1;
	//Players[playerid].BodyPartIds[Sword_PlayerId_Head] = AddObject(new Object(this));
	//ObjectArray[Players[playerid].BodyPartIds[Sword_PlayerId_Head]]->Init(this, OffsetX, OffsetY + LegHeight, OffsetZ);
	//ObjectArray[Players[playerid].BodyPartIds[Sword_PlayerId_Head]]->Mesh = 1;
	//Players[playerid].BodyPartIds[Sword_PlayerId_Body] = AddObject(new Object(this));
}