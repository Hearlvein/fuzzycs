#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

typedef sf::Vector2f v2;


struct Line : public sf::Drawable
{
	Line(v2 a, v2 b) : vert(sf::PrimitiveType::LineStrip)
	{
		for (float t = 0; t < 1; t+=0.01f)
		{
			vert.append(sf::Vertex({a.x + (b.x-a.x)*t, a.y + (b.y-a.y)*t}, sf::Color::Yellow));
		}
	}

	void draw(sf::RenderTarget& rt, sf::RenderStates st) const override { rt.draw(vert, st); }

	int pts = 100;
	sf::VertexArray vert;
};