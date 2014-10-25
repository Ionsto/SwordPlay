#include "Object.h"
#include "GameManager.h"

Object::Object(GameManager * gm,int mesh)
{
	//Type = 0;
	LOD = 0;
	PlayerId = -1;
	Mesh = mesh;
	Node = gm->smgr->addAnimatedMeshSceneNode(gm->world->Meshes[Mesh]);
	Node->setMaterialTexture(0, gm->driver->getTexture("./Res/Arm.png"));
	//Node->setPosition(vector3df(rand() % 30,0, rand() % 30));
	//Node->setRotation(vector3df(90, 0, 0));
	Node->setFrameLoop(0, 60);
	AnimationCount = 4;
	Animations = new Animation[AnimationCount];
	//Idel
	Animations[0] = Animation(0, 0, 60);
	//Walk
	Animations[1] = Animation(1, 60, 60);
	//Run
	Animations[2] = Animation(2, 120, 60);
	//Attack
	Animations[3] = Animation(3, 180, 60);
	CurrentAnimation = 0;
	AnimationStack = new short[4];
	for (int i = 0; i < AnimationCount;++i)
	{
		AnimationStack[i] = -1;
	}
	AnimationStack[0] = 1;
	AnimationStack[1] = 2;
	AnimationStack[2] = 3;
}


Object::~Object()
{
}
void Object::Update(GameManager * gm)
{
	UpdateAnimation(gm);
}
void Object::UpdateAnimation(GameManager * gm)
{
	//
	int RequestedState = 0;
	//Get New Animation
	if (GetAnimationStack(CurrentAnimation) == -1)
	{
		if (GetAnimationStack(CurrentAnimation + 1) == -1)
		{
			// 2^7 = 128 - bitmask for loopendness?
			SetAnimationStack(CurrentAnimation + 1, 0);
			CurrentAnimation += 1;
			if (CurrentAnimation > AnimationCount)
			{
				CurrentAnimation -= AnimationCount;
			}
		}
	}
	if (Node->getFrameNr() >= Animations[GetAnimationStack(CurrentAnimation)].Start && Node->getFrameNr() < Animations[GetAnimationStack(CurrentAnimation)].Start + Animations[GetAnimationStack(CurrentAnimation)].Length - 1)
	{
	}
	else
	{
		if ((GetAnimationStack(CurrentAnimation) && 128) == 0 || GetAnimationStack(CurrentAnimation) != -1)
		{
			int Frame = Node->getFrameNr();
			if (Frame == Animations[GetAnimationStack(CurrentAnimation)].Start + Animations[GetAnimationStack(CurrentAnimation)].Length - 1)
			{
				CurrentAnimation += 1;
				if (CurrentAnimation > AnimationCount)
				{
					CurrentAnimation -= AnimationCount;
				}
			}
			else
			{
				Node->setFrameLoop(Animations[GetAnimationStack(CurrentAnimation)].Start, Animations[GetAnimationStack(CurrentAnimation)].Start + Animations[GetAnimationStack(CurrentAnimation)].Length);
				Node->setCurrentFrame(Animations[GetAnimationStack(CurrentAnimation)].Start);
			}
		}
	}
}
short Object::GetAnimationStack(int position)
{
	if (position >= AnimationCount)
	{
		return GetAnimationStack(position - AnimationCount);
	}
	if (position < 0)
	{
		return GetAnimationStack(position + AnimationCount);
	}
	return AnimationStack[position];
}
void Object::SetAnimationStack(int position,short value)
{
	if (position >= AnimationCount)
	{
		SetAnimationStack(position - AnimationCount,value);
	}
	if (position < 0)
	{
		SetAnimationStack(position + AnimationCount,value);
	}
	AnimationStack[position] = value;
}