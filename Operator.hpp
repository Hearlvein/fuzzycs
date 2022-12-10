#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

typedef sf::Vector2f v2;


class Operator : public Component
{
public:
	Operator()
	{
		in1 = in2 = -1;

		m_Shape = std::make_unique<sf::RectangleShape>();
		auto rect = reinterpret_cast<sf::RectangleShape*>(m_Shape.get());
		rect->setSize({BOX_W, BOX_W});

		m_Shape->setFillColor({13, 98, 140});
		m_Shape->setOrigin(BOX_W/2, BOX_W/2);
		m_Shape->setOutlineThickness(-5);
		SetOutlineColor(GetOutlineColor());

		m_Anchor1.setRadius(ANCHOR_RAD);
		m_Anchor1.setOrigin(ANCHOR_RAD, ANCHOR_RAD);
		m_Anchor1.setFillColor(sf::Color::Magenta);
		m_Anchor2.setRadius(ANCHOR_RAD);
		m_Anchor2.setOrigin(ANCHOR_RAD, ANCHOR_RAD);
		m_Anchor2.setFillColor(sf::Color::Magenta);

		Component::SetPosition({500, 300});
		UpdateAnchorsPositions();
	}

	[[nodiscard]] sf::Color GetOutlineColor() const { return IsConnected() ? VALID_OUTLINE : UNDEF_OUTLINE; }
	[[nodiscard]] bool IsConnected() const { return in1 != -1 && in2 != -1; }
	[[nodiscard]] v2 GetAnchor1() const { return GetPosition()+m_Anchor1Offset; }
	[[nodiscard]] v2 GetAnchor2() const { return GetPosition()+m_Anchor2Offset; }

	void SetPosition(const v2& pos) override
	{
		Component::SetPosition(pos);
		UpdateAnchorsPositions();
	}

	void UpdateAnchorsPositions()
	{
		m_Anchor1.setPosition(GetAnchor1());
		m_Anchor2.setPosition(GetAnchor2());
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		Component::draw(target, states);
		target.draw(m_Anchor1);
		target.draw(m_Anchor2);
	}

private:
	const float BOX_W = 100.f;
	const sf::Color UNDEF_OUTLINE = sf::Color::Red;
	const sf::Color VALID_OUTLINE = sf::Color::Green;

	int in1;
	int in2;
	const float ANCHOR_RAD = 5.f;
	sf::CircleShape m_Anchor1;
	sf::CircleShape m_Anchor2;
	const v2 m_Anchor1Offset = {-BOX_W/2, -20};
	const v2 m_Anchor2Offset = {-BOX_W/2, 20};
};