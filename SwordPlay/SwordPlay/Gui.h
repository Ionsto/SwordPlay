#pragma once
#include <Irrlicht\irrlicht.h>
class GameManager;
using namespace irr;
using namespace core;
using namespace gui;
class Gui
{
public:
	bool Render;
	float XRatio, YRatio;
	int ElementCount;
	IGUIElement * Elements;
	Gui(){};
	Gui(IGUIEnvironment * guienv, float XRatio, float YRatio);
	~Gui();
	void StartDisplay();
	void StopDisplay();
	void Update(GameManager * gm);
	rect<s32> MidRec(float xmid, float ymid, float tw, float th)
	{
		tw /= 2;
		th /= 2;
		return rect<s32>((xmid - tw)*XRatio, (ymid - th)*YRatio, (xmid + tw)*XRatio, (ymid + th)*YRatio);
	};
};

