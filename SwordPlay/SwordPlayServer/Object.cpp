#include "Object.h"
#include "World.h"
#include <iostream>

Object::Object(World * world, bool init, float x, float y, float z)
{
	QuedMovePos.Set(0, 0, 0);
	QuedMoveRot.Set(0, 0, 0);
	Mesh = 1;
	if (init){
		Init(world,x,y,z);
	}
	PrevPos.Set(0, 0, 0);
	PrevRot.Set(0, 0, 0);
}


Object::~Object()
{
}

void Object::Init(World * world,float x,float y,float z,float sx,float sy,float sz)
{
	PhysicsBody = world->m_Sim->CreateRigidBody();
	neV3 pos; pos.Set(x, y, z);
	PhysicsBody->SetPos(pos);
	QuedMovePos.Set(0, 0, 0);
	QuedMoveRot.Set(0, 0, 0);
	neV3 size; size.Set(sx, sy, sz);
	geom = PhysicsBody->AddGeometry();
	geom->SetBoxSize(size);
	geom->SetMaterialIndex(1);
	PhysicsBody->UpdateBoundingInfo();
	float mass;
	mass = 10.0f;
	PhysicsBody->SetInertiaTensor(neBoxInertiaTensor(size, mass));
	PhysicsBody->SetMass(mass);
	PhysicsBody->SetAngularDamping(0.1f);

}
void Object::Update(World * world)
{
	if (QuedMovePos.X() != 0 || QuedMovePos.Y() != 0 || QuedMovePos.Z() != 0)
	{
		PhysicsBody->ApplyImpulse(QuedMovePos);
	}
	if (QuedMoveRot.X() != 0 || QuedMoveRot.Y() != 0 || QuedMoveRot.Z() != 0)
	{
		PhysicsBody->ApplyTwist(QuedMoveRot);
	}
	QuedMovePos.Set(0, 0, 0);
	QuedMoveRot.Set(0, 0, 0);
	UpdateRotation();
	NormaliseEulerRotation();
	//std::cout << PhysicsBody->GetPos()[0] << "," << PhysicsBody->GetPos()[1] << "," << PhysicsBody->GetPos()[2] << "," << "\n";
}
void Object::NormaliseEulerRotation()
{
	if (RotationEuler.X() > 360)
	{
		RotationEuler[0] -= 360; }
	if (RotationEuler.Y() > 360)
	{
		RotationEuler[1] -= 360; }
	if (RotationEuler.Z() > 360)
	{ 
		RotationEuler[2] -= 360; }
	if (RotationEuler.X() < 0)
	{
		RotationEuler[0] += 360; }
	if (RotationEuler.Y() < 0)
	{ 
		RotationEuler[1] += 360; }
	if (RotationEuler.Z() < 0)
	{
		RotationEuler[2] += 360; }
}
void Object::SetLocation(float x, float y, float z)
{
	neV3 pos; pos.Set(x, y, z);
	PhysicsBody->SetPos(pos);
	UpdateRotation();
}
void Object::SetRotation(float x, float y, float z)
{
	neV3 pos; pos.Set(x, y, z);
	neQ rot; rot.Set(pos, 1);
	PhysicsBody->SetRotation(rot);
}
void Object::UpdateRotation()
{
	neQ rot = PhysicsBody->GetRotationQ();
	double sqw = rot.W*rot.W;
	double sqx = rot.X*rot.X;
	double sqy = rot.Y*rot.Y;
	double sqz = rot.Z*rot.Z;
	double unit = sqx + sqy + sqz + sqw; // if normalised is one, otherwise is correction factor
	double test = rot.X*rot.Y + rot.Z*rot.W;
	float heading = 0;
	float attitude = 0;
	float bank = 0;
	if (test > 0.499*unit) { // singularity at north pole
		heading = 2 * atan2(rot.X, rot.W);
		attitude = 3.14 / 2;
		bank = 0;
	}else if (test < -0.499*unit) { // singularity at south pole
		heading = -2 * atan2(rot.X, rot.W);
		attitude = -3.14 / 2;
		bank = 0;
	}
	else
	{
		heading = atan2(2 * rot.X*rot.W - 2 * rot.Y*rot.Z, -sqx + sqy - sqz + sqw);
		attitude = asin(2 * test / unit);
		bank = atan2(2 * rot.Y*rot.W - 2 * rot.X*rot.Z, sqx - sqy - sqz + sqw);
	}
	RotationEuler.Set(bank, heading, attitude);
}
void Object::Destroy(World * world)
{
	world->ObjectArray[Id] = NULL;
	world->m_Sim->FreeRigidBody(PhysicsBody);
	delete this;
}