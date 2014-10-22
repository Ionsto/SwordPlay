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
}


Object::~Object()
{
}

void Object::Init(World * world,float x,float y,float z)
{
	PhysicsBody = world->m_Sim->CreateRigidBody();
	neV3 pos; pos.Set(x, y, z);
	PhysicsBody->SetPos(pos);
	QuedMovePos.Set(0, 0, 0);
	QuedMoveRot.Set(0, 0, 0);
	neV3 size; size.Set(5, 5, 5);
	geom = PhysicsBody->AddGeometry();
	geom->SetBoxSize(size);
	geom->SetMaterialIndex(1);
	PhysicsBody->UpdateBoundingInfo();
	float mass;
	mass = 1.0f;
	PhysicsBody->SetInertiaTensor(neBoxInertiaTensor(size, mass));
	PhysicsBody->SetMass(mass);
	PhysicsBody->SetAngularDamping(0.01f);

}
void Object::Update(World * world)
{
	if (QuedMovePos.X() != 0 || QuedMovePos.W() != 0 || QuedMovePos.Z() != 0)
	{
		PhysicsBody->ApplyImpulse(QuedMovePos);
	}
	PhysicsBody->ApplyTwist(QuedMoveRot);
	QuedMovePos.Set(0, 0, 0);
	QuedMoveRot.Set(0, 0, 0);
	//std::cout << PhysicsBody->GetPos()[0] << "," << PhysicsBody->GetPos()[1] << "," << PhysicsBody->GetPos()[2] << "," << "\n";
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
	if (test > 0.499*unit) { // singularity at north pole
		RotationEuler.Set(0, 2 * atan2(rot.X, rot.W), 3.14 / 2);
		return;
	}
	if (test < -0.499*unit) { // singularity at south pole
		RotationEuler.Set(0, -2 * atan2(rot.X, rot.W), -3.14 / 2);
		return;
	}
	RotationEuler.Set(atan2(2 * rot.X*rot.W - 2 * rot.Y*rot.Z, -sqx + sqy - sqz + sqw), asin(2 * test / unit), atan2(2 * rot.Y*rot.W - 2 * rot.X*rot.Z, sqx - sqy - sqz + sqw));
}