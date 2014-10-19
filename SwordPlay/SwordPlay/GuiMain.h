#pragma once
#include "Gui.h"
class GuiMain :public Gui
{
public:
	IGUIButton * StartButton;
	IGUIButton * ExitButton;
	GuiMain(){};
	GuiMain(IGUIEnvironment * guienv, float XRatio, float YRatio);
	~GuiMain();
};

