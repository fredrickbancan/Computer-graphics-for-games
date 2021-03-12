#include "Input.h"
#include <GLFW/glfw3.h>

namespace aie {

Input* Input::m_instance = nullptr;

Input::Input() {

	// track current/previous key and mouse button states
	m_lastKeys = new int[GLFW_KEY_LAST + 1];
	m_currentKeys = new int[GLFW_KEY_LAST + 1];

	auto window = glfwGetCurrentContext();

	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
		m_lastKeys[i] = m_currentKeys[i] = glfwGetKey(window, i);

	for (int i = 0; i < 8; ++i)
		m_lastButtons[i] = m_currentButtons[i] = glfwGetMouseButton(window, i);

	// set up callbacks
	auto KeyPressCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {

		for (auto& f : Input::getInstance()->m_keyCallbacks)
			f(window, key, scancode, action, mods);
	};

	auto CharacterInputCallback = [](GLFWwindow* window, unsigned int character) {

		Input::getInstance()->m_pressedCharacters.push_back(character);

		for (auto& f : Input::getInstance()->m_charCallbacks)
			f(window, character);
	};

	auto MouseMoveCallback = [](GLFWwindow* window, double x, double y) {
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);

		Input::getInstance()->onMouseMove((int)x, h - (int)y);

		for (auto& f : Input::getInstance()->m_mouseMoveCallbacks)
			f(window, x, h - y);
	};

	auto MouseInputCallback = [](GLFWwindow* window, int button, int action, int mods) {
		
		for (auto& f : Input::getInstance()->m_mouseButtonCallbacks)
			f(window, button, action, mods);
	};

	auto MouseScrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {

		Input::getInstance()->m_mouseScroll += yoffset;

		for (auto& f : Input::getInstance()->m_mouseScrollCallbacks)
			f(window, xoffset, yoffset);
	};

	auto MouseEnterCallback = [](GLFWwindow* window, int entered) {
		// Set flag to prevent large mouse delta on entering screen
		Input::getInstance()->m_firstMouseMove = true;
	};

	glfwSetKeyCallback(window, KeyPressCallback);
	glfwSetCharCallback(window, CharacterInputCallback);
	glfwSetMouseButtonCallback(window, MouseInputCallback);
	glfwSetCursorPosCallback(window, MouseMoveCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);
	glfwSetCursorEnterCallback(window, MouseEnterCallback);
	
	m_mouseX = 0;
	m_mouseY = 0;
	m_mouseScroll = 0;
}

Input::~Input() {
	delete[] m_lastKeys;
	delete[] m_currentKeys;
}

void Input::onMouseMove(int newXPos, int newYPos) {
	m_mouseX = newXPos;
	m_mouseY = newYPos;
	if (m_firstMouseMove) {
		// On first move after startup/entering window reset old mouse position
		m_oldMouseX = newXPos;
		m_oldMouseY = newYPos;
		m_firstMouseMove = false;
	}
}

void Input::clearStatus() {

	m_pressedCharacters.clear();

	auto window = glfwGetCurrentContext();

	m_pressedKeys.clear();

	// update keys
	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i) {

		m_lastKeys[i] = m_currentKeys[i];

		if ((m_currentKeys[i] = glfwGetKey(window, i)) == GLFW_PRESS)
			m_pressedKeys.push_back(m_currentKeys[i]);
	}

	// update mouse
	for (int i = 0; i < 8 ; ++i) {
		m_lastButtons[i] = m_currentButtons[i];
		m_currentButtons[i] = glfwGetMouseButton(window, i);
	}

	// update old mouse position
	m_oldMouseX = m_mouseX;
	m_oldMouseY = m_mouseY;
}

bool Input::isKeyDown(int inputKeyID) {
	return m_currentKeys[inputKeyID] == GLFW_PRESS;
}

bool Input::isKeyUp(int inputKeyID) {
	return m_currentKeys[inputKeyID] == GLFW_RELEASE;
}

bool Input::wasKeyPressed(int inputKeyID) {
	return m_currentKeys[inputKeyID] == GLFW_PRESS && 
		m_lastKeys[inputKeyID] == GLFW_RELEASE;
}

bool Input::wasKeyReleased(int inputKeyID) {
	return m_currentKeys[inputKeyID] == GLFW_RELEASE && 
		m_lastKeys[inputKeyID] == GLFW_PRESS;
}

const std::vector<int> &Input::getPressedKeys() const {
	return m_pressedKeys;
}

const std::vector<unsigned int> &Input::getPressedCharacters() const {
	return m_pressedCharacters;
}

bool Input::isMouseButtonDown(int inputMouseID) {
	return m_currentButtons[inputMouseID] == GLFW_PRESS;
}

bool Input::isMouseButtonUp(int inputMouseID) {
	return m_currentButtons[inputMouseID] == GLFW_RELEASE;
}

bool Input::wasMouseButtonPressed(int inputMouseID) {
	return m_currentButtons[inputMouseID] == GLFW_PRESS && 
		m_lastButtons[inputMouseID] == GLFW_RELEASE;
}

bool Input::wasMouseButtonReleased(int inputMouseID) {
	return m_currentButtons[inputMouseID] == GLFW_RELEASE && 
		m_lastButtons[inputMouseID] == GLFW_PRESS;
}

int Input::getMouseX() {
	return m_mouseX;
}

int Input::getMouseY() {
	return m_mouseY;
}

double Input::getMouseScroll() {
	return m_mouseScroll;
}

void Input::getMouseXY(int* x, int* y) {
	if ( x != nullptr ) *x = m_mouseX;
	if ( y != nullptr) *y = m_mouseY;
}

int Input::getMouseDeltaX()
{
	return m_mouseX - m_oldMouseX;
}

int Input::getMouseDeltaY()
{
	return m_mouseY - m_oldMouseY;
}

void Input::getMouseDelta(int * x, int * y)
{
	if (x != nullptr ) *x = m_mouseX - m_oldMouseX;
	if (y != nullptr) *y = m_mouseY - m_oldMouseY;
}

} // namespace aie