#include "MouseHelper.h"

MouseHelper* MouseHelper::instance = nullptr;

void MouseHelper::update(float windowCenterX, float windowCenterY, float newMouseX, float newMouseY)
{
	xDelta = newMouseX - windowCenterX;
	yDelta = newMouseY - windowCenterY;
}
