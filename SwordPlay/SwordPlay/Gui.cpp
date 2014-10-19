#include "Gui.h"
#include "GameManager.h"

Gui::Gui(IGUIEnvironment * guienv, float XRatio, float YRatio)
{
	this->XRatio = XRatio;
	this->YRatio = YRatio;
	ElementCount = 0;
	Render = false;
}


Gui::~Gui()
{
}

void Gui::StartDisplay()
{
	for (int i = 0;i< ElementCount;++i)
	{
		Elements[i].setVisible(true);
	}
	Render = true;
}

void Gui::StopDisplay()
{
	for (int i = 0; i< ElementCount; ++i)
	{
		Elements[i].setVisible(false);
	}
	Render = false;
}

void Gui::Update(GameManager * gm)
{

}