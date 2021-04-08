#pragma once

/*Static class for helping to accurately get mouse deltas for camera*/
class MouseHelper
{
public:
	static void init() { if(instance == nullptr) instance = new MouseHelper(); }
	static void close() { if(instance != nullptr) delete instance; }
	static MouseHelper* get() { return instance; }
	void update(float windowCenterX, float windowCenterY, float newMouseX, float newMouseY);

	float getXDelta() { return xDelta; }
	float getYDelta() { return yDelta; }

private:
	static MouseHelper* instance;
	float xDelta = 0;
	float yDelta = 0;
};