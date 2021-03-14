#pragma once

class MouseHelper
{
public:
	static void init() { if(instance == nullptr) instance = new MouseHelper(); }
	static void close() { if(instance != nullptr) delete instance; }
	static MouseHelper* get() { return instance; }
	void update(int windowCenterX, int windowCenterY, int newMouseX, int newMouseY);

	int getXDelta() { return xDelta; }
	int getYDelta() { return yDelta; }

private:
	static MouseHelper* instance;
	int xDelta = 0;
	int yDelta = 0;
};