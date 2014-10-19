#include "GuiMain.h"


GuiMain::GuiMain(IGUIEnvironment * guienv, float XRatio, float YRatio) : Gui(guienv,XRatio,YRatio)
{
	StartButton = guienv->addButton(MidRec(0, 200, 100, 100), NULL, NULL, L"Ai vs Ai game");
	ElementCount = 1;
}


GuiMain::~GuiMain()
{
}
