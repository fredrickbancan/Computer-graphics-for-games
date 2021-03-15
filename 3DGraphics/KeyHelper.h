#pragma once

class KeyHelper
{
public:
	static void init() { if (instance == nullptr) instance = new KeyHelper(); }
	static void close() { if (instance != nullptr) delete instance; }
	static KeyHelper* get() { return instance; }

	KeyHelper();
	~KeyHelper();
	/*Should be called after each loop of logic. Prepares object for detecting new frame inputs.*/
	void reset();

	/*Returns true if the key was pressed in the frame this function was called, otherwise returns false.
	  Useful for input which you only want to actuate once like mouse clicks and opening menus/pausing app.*/
	bool keyPressedOnce(int inputKeyID);

	/*Returns true if the provided key ID is a mouse button. Otherwise returns false.*/
	static bool isKeyMouseButton(int inputKeyID);

private:
	static KeyHelper* instance;
	bool* keyBoardArray = nullptr;
	bool* mouseKeyArray = nullptr;
};