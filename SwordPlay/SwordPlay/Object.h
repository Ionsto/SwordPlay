#pragma once
#include <Irrlicht\irrlicht.h>
#include <Irrlicht\ISceneNode.h>
#include <Irrlicht\IMeshSceneNode.h>
class GameManager;
using namespace irr;
using namespace scene;
struct Animation
{
	int Type;
	int Id;
	int Start;
	int Length;
	Animation(){};
	Animation(int id, int start, int length)
	{
		Id = id;
		Start = start;
		Length = length;
	}
};
class Object
{
public:
	IAnimatedMeshSceneNode * Node;//Location
	int LOD;
	int PlayerId;
	int Mesh;
	//Location
	int WorldPart;
	int AnimationCount;
	Animation * Animations;
	short * AnimationStack;
	short CurrentAnimation;
	Object(GameManager * gm,int);
	~Object();
	void Update(GameManager * gm);
	void UpdateAnimation(GameManager * gm);
	short GetAnimationStack(int position);
	void SetAnimationStack(int position,short value);
};

