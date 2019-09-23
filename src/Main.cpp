#include <iostream>
#include <SFML/Graphics.hpp>
#include <sol/sol.hpp>

int main(int argc, char** argv)
{
	sf::RenderWindow window(sf::VideoMode(400, 400), "TEST");
	sol::state lua;

	lua.open_libraries(sol::lib::base);

	lua.script("print(\"hello\")");


	while (window.isOpen())
	{
		sf::Event ev;
		while (window.pollEvent(ev))
		{
			switch (ev.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		window.clear();
		window.display();
	}

	return 0;
}