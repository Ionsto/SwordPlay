#pragma once
#include <Tokamak\tokamak.h>
class World;
class Object
{
public:
	int Id;
	int Mesh;
	neV3 QuedMovePos;
	neV3 QuedMoveRot;
	neV3 RotationEuler;
	neGeometry * geom;
	neRigidBody * PhysicsBody;
	Object(World * world, bool init = false, float x = 0, float y = 0, float z = 0);
	~Object();
	void Init(World * world, float x = 0, float y = 0, float z = 0);
	void Update(World * world);
	void SetLocation(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void UpdateRotation();
	void Destroy(World * world);
	void NormaliseEulerRotation();
};

