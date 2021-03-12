#pragma once

#include <vector>
#include <functional>
#include <map>

struct GLFWwindow;

namespace aie {

// a giant list of input codes for keyboard and mouse
enum EInputCodes : int {
	INPUT_KEY_UNKNOWN			= -1,
	INPUT_KEY_SPACE				= 32,
	INPUT_KEY_APOSTROPHE		= 39,
	INPUT_KEY_COMMA				= 44,
	INPUT_KEY_MINUS				= 45,
	INPUT_KEY_PERIOD			= 46,
	INPUT_KEY_SLASH				= 47,
	INPUT_KEY_0					= 48,
	INPUT_KEY_1					= 49,
	INPUT_KEY_2					= 50,
	INPUT_KEY_3					= 51,
	INPUT_KEY_4					= 52,
	INPUT_KEY_5					= 53,
	INPUT_KEY_6					= 54,
	INPUT_KEY_7					= 55,
	INPUT_KEY_8					= 56,
	INPUT_KEY_9					= 57,
	INPUT_KEY_SEMICOLON			= 59,
	INPUT_KEY_EQUAL				= 61,
	INPUT_KEY_A					= 65,
	INPUT_KEY_B					= 66,
	INPUT_KEY_C					= 67,
	INPUT_KEY_D					= 68,
	INPUT_KEY_E					= 69,
	INPUT_KEY_F					= 70,
	INPUT_KEY_G					= 71,
	INPUT_KEY_H					= 72,
	INPUT_KEY_I					= 73,
	INPUT_KEY_J					= 74,
	INPUT_KEY_K					= 75,
	INPUT_KEY_L					= 76,
	INPUT_KEY_M					= 77,
	INPUT_KEY_N					= 78,
	INPUT_KEY_O					= 79,
	INPUT_KEY_P					= 80,
	INPUT_KEY_Q					= 81,
	INPUT_KEY_R					= 82,
	INPUT_KEY_S					= 83,
	INPUT_KEY_T					= 84,
	INPUT_KEY_U					= 85,
	INPUT_KEY_V					= 86,
	INPUT_KEY_W					= 87,
	INPUT_KEY_X					= 88,
	INPUT_KEY_Y					= 89,
	INPUT_KEY_Z					= 90,
	INPUT_KEY_LEFT_BRACKET		= 91,
	INPUT_KEY_BACKSLASH			= 92,
	INPUT_KEY_RIGHT_BRACKET		= 93,
	INPUT_KEY_GRAVE_ACCENT		= 96,
	INPUT_KEY_ESCAPE			= 256,
	INPUT_KEY_ENTER				= 257,
	INPUT_KEY_TAB				= 258,
	INPUT_KEY_BACKSPACE			= 259,
	INPUT_KEY_INSERT			= 260,
	INPUT_KEY_DELETE			= 261,
	INPUT_KEY_RIGHT				= 262,
	INPUT_KEY_LEFT				= 263,
	INPUT_KEY_DOWN				= 264,
	INPUT_KEY_UP				= 265,
	INPUT_KEY_PAGE_UP			= 266,
	INPUT_KEY_PAGE_DOWN			= 267,
	INPUT_KEY_HOME				= 268,
	INPUT_KEY_END				= 269,
	INPUT_KEY_CAPS_LOCK			= 280,
	INPUT_KEY_SCROLL_LOCK		= 281,
	INPUT_KEY_NUM_LOCK			= 282,
	INPUT_KEY_PRINT_SCREEN		= 283,
	INPUT_KEY_PAUSE				= 284,
	INPUT_KEY_F1				= 290,
	INPUT_KEY_F2				= 291,
	INPUT_KEY_F3				= 292,
	INPUT_KEY_F4				= 293,
	INPUT_KEY_F5				= 294,
	INPUT_KEY_F6				= 295,
	INPUT_KEY_F7				= 296,
	INPUT_KEY_F8				= 297,
	INPUT_KEY_F9				= 298,
	INPUT_KEY_F10				= 299,
	INPUT_KEY_F11				= 300,
	INPUT_KEY_F12				= 301,
	INPUT_KEY_F13				= 302,
	INPUT_KEY_F14				= 303,
	INPUT_KEY_F15				= 304,
	INPUT_KEY_F16				= 305,
	INPUT_KEY_F17				= 306,
	INPUT_KEY_F18				= 307,
	INPUT_KEY_F19				= 308,
	INPUT_KEY_F20				= 309,
	INPUT_KEY_F21				= 310,
	INPUT_KEY_F22				= 311,
	INPUT_KEY_F23				= 312,
	INPUT_KEY_F24				= 313,
	INPUT_KEY_F25				= 314,
	INPUT_KEY_KP_0				= 320,
	INPUT_KEY_KP_1				= 321,
	INPUT_KEY_KP_2				= 322,
	INPUT_KEY_KP_3				= 323,
	INPUT_KEY_KP_4				= 324,
	INPUT_KEY_KP_5				= 325,
	INPUT_KEY_KP_6				= 326,
	INPUT_KEY_KP_7				= 327,
	INPUT_KEY_KP_8				= 328,
	INPUT_KEY_KP_9				= 329,
	INPUT_KEY_KP_DECIMAL		= 330,
	INPUT_KEY_KP_DIVIDE			= 331,
	INPUT_KEY_KP_MULTIPLY		= 332,
	INPUT_KEY_KP_SUBTRACT		= 333,
	INPUT_KEY_KP_ADD			= 334,
	INPUT_KEY_KP_ENTER			= 335,
	INPUT_KEY_KP_EQUAL			= 336,
	INPUT_KEY_LEFT_SHIFT		= 340,
	INPUT_KEY_LEFT_CONTROL		= 341,
	INPUT_KEY_LEFT_ALT			= 342,
	INPUT_KEY_RIGHT_SHIFT		= 344,
	INPUT_KEY_RIGHT_CONTROL		= 345,
	INPUT_KEY_RIGHT_ALT			= 346,
	INPUT_MOUSE_BUTTON_LEFT		= 0,
	INPUT_MOUSE_BUTTON_RIGHT	= 1,
	INPUT_MOUSE_BUTTON_MIDDLE	= 2,
	INPUT_MOUSE_BUTTON_4		= 3,
	INPUT_MOUSE_BUTTON_5		= 4,
	INPUT_MOUSE_BUTTON_6		= 5,
	INPUT_MOUSE_BUTTON_7		= 6,
	INPUT_MOUSE_BUTTON_8		= 7,
};

// a singleton class that manages Input from the keyboard and mouse
class Input {
public:

	// returns access to the singleton instance
	static Input* getInstance() { return m_instance;  }

	// query the keyboard state
	bool isKeyDown(int inputKeyID);
	bool isKeyUp(int inputKeyID);

	// returns true if the key was pressed / released this frame
	bool wasKeyPressed(int inputKeyID);
	bool wasKeyReleased(int inputKeyID);

	// returns access to all keys that are currently pressed
	const std::vector<int>& getPressedKeys() const;
	const std::vector<unsigned int>& getPressedCharacters() const;

	// query the mouse button state
	bool isMouseButtonDown(int inputMouseID);
	bool isMouseButtonUp(int inputMouseID);

	// returns true if the button was pressed / released this frame
	bool wasMouseButtonPressed(int inputMouseID);
	bool wasMouseButtonReleased(int inputMouseID);

	// query the mouse position
	int getMouseX();
	int getMouseY();
	void getMouseXY(int* x, int* y);

	// query mouse movement
	int getMouseDeltaX();
	int getMouseDeltaY();
	void getMouseDelta(int* x, int* y);

	// query how far the mouse wheel has been moved 
	double getMouseScroll();

	// delgates for attaching input observers to the Input class
	typedef std::function<void(GLFWwindow* window, int key, int scancode, int action, int mods)> KeyCallback;
	typedef std::function<void(GLFWwindow* window, unsigned int character)> CharCallback;
	typedef std::function<void(GLFWwindow* window, int button, int action, int mods)> MouseButtonCallback;
	typedef std::function<void(GLFWwindow* window, double xoffset, double yoffset)> MouseScrollCallback;
	typedef std::function<void(GLFWwindow* window, double x, double y)> MouseMoveCallback;

	// functions for attatching input observers
	void attachKeyObserver(const KeyCallback& callback) { m_keyCallbacks.push_back(callback); }
	void attachCharObserver(const CharCallback& callback) { m_charCallbacks.push_back(callback); }
	void attachMouseButtonObserver(const MouseButtonCallback& callback) { m_mouseButtonCallbacks.push_back(callback); }
	void attachMouseMoveObserver(const MouseMoveCallback& callback) { m_mouseMoveCallbacks.push_back(callback); }
	void attachMouseScrollObserver(const MouseScrollCallback& callback) { m_mouseScrollCallbacks.push_back(callback); }

protected:

	// just giving the Application class access to the Input singleton
	friend class Application;

	// singleton pointer
	static Input* m_instance;

	// only want the Application class to be able to create / destroy
	static void create()			{ m_instance = new Input(); }
	static void destroy()			{ delete m_instance; }

	// should be called once by the application each frame after the current update
	// or before glfwPollEvents
	void clearStatus();

private:

	// constructor private for singleton
	Input();
	~Input();

	std::vector<int>			m_pressedKeys;
	std::vector<unsigned int>	m_pressedCharacters;
		
	int		m_mouseX;
	int		m_mouseY;
	int		m_oldMouseX;
	int		m_oldMouseY;
	double	m_mouseScroll;

	bool	m_firstMouseMove;	// flag for first mouse input after start or mouse entering window

	void onMouseMove(int newXPos, int newYPos);
	
	std::vector<KeyCallback>			m_keyCallbacks;
	std::vector<CharCallback>			m_charCallbacks;
	std::vector<MouseMoveCallback>		m_mouseMoveCallbacks;
	std::vector<MouseButtonCallback>	m_mouseButtonCallbacks;
	std::vector<MouseScrollCallback>	m_mouseScrollCallbacks;

	// used to track down/up/released/pressed
	int* m_lastKeys, *m_currentKeys;
	int m_lastButtons[8], m_currentButtons[8];
};

} // namespace aie