#include "World.h"


World::World()
{
	neSimulatorSizeInfo sizeInfo;
	RigidBodyCount = 100;
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
}


World::~World()
{
	neSimulator::DestroySimulator(m_Sim);
}
