#include "MouseHelper.h"

MouseHelper* MouseHelper::instance = nullptr;

void MouseHelper::update(int windowCenterX, int windowCenterY, int newMouseX, int newMouseY)
{
	xDelta = newMouseX - windowCenterX;
	yDelta = newMouseY - windowCenterY;
}
