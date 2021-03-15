#include "KeyHelper.h"
#include "Input.h"

KeyHelper* KeyHelper::instance = nullptr;

KeyHelper::KeyHelper()
{
	keyBoardArray = new bool[512];
	mouseKeyArray = new bool[32];
}

KeyHelper::~KeyHelper()
{

}

void KeyHelper::reset()
{

}

bool KeyHelper::keyPressedOnce(int inputKeyID)
{
	if (isKeyMouseButton(inputKeyID))
	{

	}
	else
	{

	}

	return false;
}

bool KeyHelper::isKeyMouseButton(int inputKeyID)
{
	return inputKeyID >= aie::INPUT_MOUSE_BUTTON_LEFT && inputKeyID <= aie::INPUT_MOUSE_BUTTON_8;
}
