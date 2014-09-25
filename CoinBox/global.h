#ifndef _GLOBAL
#define _GLOBAL

#include <Windows.h>
#include <SFML\Graphics.hpp>

#define APP_NAME L"Копилка"
#define APP_ERROR_TITLE L"Копилка: Error"
#define APP_WIDTH 500
#define APP_HEIGHT 225
#define OFFSET_Y 15
#define TIMER_MAX 30

class System {
public:
	static sf::RenderWindow* window;

	static void error(std::wstring message) {
		MessageBox(window->getSystemHandle(), message.c_str(), APP_ERROR_TITLE, 0);
		exit();
	}

	static void exit() {
		window->close();
	}
};

#endif