#include <Windows.h>
#include <fstream>

#include <SFML\Graphics.hpp>

#include "global.h"
#include "input.h"

#ifdef _DEBUG
	#pragma comment (lib,"sfml-system-d.lib")
	#pragma comment (lib,"sfml-window-d.lib")
	#pragma comment (lib,"sfml-graphics-d.lib")
#else
	#pragma comment (lib,"sfml-system.lib")
	#pragma comment (lib,"sfml-window.lib")
	#pragma comment (lib,"sfml-graphics.lib")
#endif

int money;
int timer;

sf::Font font;

sf::Text text;
sf::Text textAdd1;
sf::Text textAdd10;
sf::Text textAdd100;
sf::Text textAdd1000;
sf::Text textSub1;
sf::Text textSub10;
sf::Text textSub100;
sf::Text textSub1000;

void open() {
	std::ifstream file("data.bin", std::ios::binary);
	if (file.is_open()) {
		file.read((char*)&money, sizeof(int));
	}
	file.close();
}

void save() {
	std::ofstream file("data.bin", std::ios::binary);
	file.write((char*)&money, sizeof(int));
	file.close();
}

void setValue(int value) {
	money = value;
	text.setString(std::to_string(money));

	sf::Vector2u winSize = System::window->getSize();
	sf::FloatRect rect = text.getGlobalBounds();
	text.setPosition(winSize.x / 2 - rect.width / 2, 30 + OFFSET_Y);

	timer = 0;
}

void resize(sf::Vector2u size) {
	if (size.x < APP_WIDTH)
		size.x = APP_WIDTH;
	
	setValue(money);

	textAdd1.setPosition(sf::Vector2f(size.x - 50, 0 + OFFSET_Y));
	textAdd10.setPosition(sf::Vector2f(size.x - 66, 50 + OFFSET_Y));
	textAdd100.setPosition(sf::Vector2f(size.x - 81, 100 + OFFSET_Y));
	textAdd1000.setPosition(sf::Vector2f(size.x - 98, 150 + OFFSET_Y));

	textSub1.setPosition(sf::Vector2f(15, 0 + OFFSET_Y));
	textSub10.setPosition(sf::Vector2f(15, 50 + OFFSET_Y));
	textSub100.setPosition(sf::Vector2f(15, 100 + OFFSET_Y));
	textSub1000.setPosition(sf::Vector2f(15, 150 + OFFSET_Y));

	size.y = APP_HEIGHT;
	System::window->setSize(size);
}

bool init() {
	if (!font.loadFromFile("c:\\Windows\\Fonts\\tahoma.ttf")) {
		return false;
	}

	open();

	text.setFont(font);
	text.setCharacterSize(100);
	text.setColor(sf::Color::Black);

	textAdd1.setFont(font);
	textAdd1.setColor(sf::Color::Black);
	textAdd1.setString("+1");

	textAdd10.setFont(font);
	textAdd10.setColor(sf::Color::Black);
	textAdd10.setString("+10");

	textAdd100.setFont(font);
	textAdd100.setColor(sf::Color::Black);
	textAdd100.setString("+100");

	textAdd1000.setFont(font);
	textAdd1000.setColor(sf::Color::Black);
	textAdd1000.setString("+1000");

	textSub1.setFont(font);
	textSub1.setColor(sf::Color::Black);
	textSub1.setString("-1");

	textSub10.setFont(font);
	textSub10.setColor(sf::Color::Black);
	textSub10.setString("-10");

	textSub100.setFont(font);
	textSub100.setColor(sf::Color::Black);
	textSub100.setString("-100");

	textSub1000.setFont(font);
	textSub1000.setColor(sf::Color::Black);
	textSub1000.setString("-1000");

	resize(System::window->getSize());

	return true;
}

bool isContains(sf::FloatRect rect, sf::Vector2i vec) {
	return vec.x >= rect.left && vec.x < rect.left + rect.width && vec.y >= rect.top && vec.y < rect.top + rect.height;
}

void update() {
	sf::Vector2i mousePos = Input::getMousePos();

	if (Input::isMouseHit(sf::Mouse::Left)) {
		if (isContains(textAdd1.getGlobalBounds(), mousePos))
			setValue(money + 1);
		else if (isContains(textAdd10.getGlobalBounds(), mousePos))
			setValue(money + 10);
		else if (isContains(textAdd100.getGlobalBounds(), mousePos))
			setValue(money + 100);
		else if (isContains(textAdd1000.getGlobalBounds(), mousePos))
			setValue(money + 1000);
		else if (isContains(textSub1.getGlobalBounds(), mousePos))
			setValue(money - 1);
		else if (isContains(textSub10.getGlobalBounds(), mousePos))
			setValue(money - 10);
		else if (isContains(textSub100.getGlobalBounds(), mousePos))
			setValue(money - 100);
		else if (isContains(textSub1000.getGlobalBounds(), mousePos))
			setValue(money - 1000);
	}

	if (timer == 60)
		save();
	if (timer <= 60)
		timer++;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	sf::RenderWindow window(sf::VideoMode(APP_WIDTH, APP_HEIGHT), APP_NAME);
	window.setFramerateLimit(60);

	System::window = &window;
	sf::View view(window.getDefaultView());
	Input::init();

	if (!init())
		return -1;

	while (window.isOpen()) {
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				save();
				window.close();
			}
			else if (e.type == sf::Event::Resized) {
				sf::Vector2u windowSize = window.getSize();
				window.setView(view = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y))));
				resize(windowSize);
			}

			Input::analyseEvent(e);
		}

		update();

		window.clear(sf::Color::White);
		{
			window.draw(text);
			window.draw(textAdd1);
			window.draw(textAdd10);
			window.draw(textAdd100);
			window.draw(textAdd1000);
			window.draw(textSub1);
			window.draw(textSub10);
			window.draw(textSub100);
			window.draw(textSub1000);
		}
		window.display();

		Input::flush();
	}

	return 0;
}