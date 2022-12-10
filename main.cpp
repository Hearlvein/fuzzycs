#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "Operator.hpp"
#include "Switch.hpp"
#include "Line.hpp"
#include <iostream>

typedef sf::Vector2f v2;

struct ScreenData
{
	ScreenData() :
		ln(op.GetAnchor1(), sw.GetAnchor())
	{
		window.create(sf::VideoMode(800, 800), "Fuzzycs!");
		window.setFramerateLimit(60);
	}

	sf::RenderWindow window;
	Operator op;
	Switch sw;
	Line ln;
	std::vector<Component*> components = {&op, &sw};
	Component* dragging = nullptr;
	v2 draggingOffset;
};


int main()
{
	ScreenData sd;

	while (sd.window.isOpen())
	{
		sf::Vector2i mousePosInt = sf::Mouse::getPosition(sd.window);
		v2 mousePos = {(float) mousePosInt.x, (float) mousePosInt.y};

		sf::Event event{};
		while (sd.window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				sd.window.close();
			}
			else if (!sd.dragging && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (const auto c : sd.components)
				{
					if (c->GetBounds().contains(mousePos))
					{
						sd.draggingOffset = c->GetPosition() - mousePos;
						sd.dragging = c;
					}
				}
			}
			else if (sd.dragging && event.type == sf::Event::MouseButtonReleased)
				sd.dragging = nullptr;
			else if (event.type == sf::Event::MouseButtonPressed && sd.sw.GetBounds().contains(mousePos)
				&& event.mouseButton.button == sf::Mouse::Right)
				sd.sw.Toggle();
		}

		if (sd.dragging)
		{
			sd.dragging->SetPosition(mousePos + sd.draggingOffset);
			sd.ln = Line(sd.op.GetAnchor1(), sd.sw.GetAnchor());
		}

		sd.window.clear();
		for (auto c : sd.components)
			sd.window.draw(*c);
		sd.window.draw(sd.ln);
		sd.window.display();
	}

	return 0;
}