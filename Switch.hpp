#pragma once

#include <SFML/Graphics.hpp>
#include "Component.hpp"

typedef sf::Vector2f v2;

class Switch : public Component
{
public:
	Switch()
	{
		m_Shape = std::make_unique<sf::CircleShape>();
		auto circ = reinterpret_cast<sf::CircleShape*>(m_Shape.get());
		circ->setRadius(RAD);

		m_Shape->setOrigin(RAD, RAD);
		m_Shape->setOutlineThickness(-3);
		m_Shape->setOutlineColor({204, 47, 144});
		UpdateColor();

		m_Anchor.setRadius(ANCHOR_RAD);
		m_Anchor.setOrigin(ANCHOR_RAD, ANCHOR_RAD);
		m_Anchor.setFillColor(sf::Color::Magenta);
		Component::SetPosition({700.f, 100.f});
		UpdateAnchorPosition();
	}

	[[nodiscard]] v2 GetAnchor() const { return GetPosition()+anchor; }
	void Toggle() { m_IsOn = !m_IsOn; UpdateColor(); }
	void UpdateColor() {
		if (m_IsOn)
			m_Shape->setFillColor(ON_COLOR);
		else
			m_Shape->setFillColor(OFF_COLOR);
	}

	void SetPosition(const v2& pos) override
	{
		Component::SetPosition(pos);
		UpdateAnchorPosition();
	}

	void UpdateAnchorPosition()
	{
		m_Anchor.setPosition(GetPosition());
	}

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		Component::draw(target, states);
		target.draw(m_Anchor);
	}

private:
	const v2 anchor = {0, 0};
	const float RAD = 30.f;
	const sf::Color ON_COLOR = sf::Color::Green;
	const sf::Color OFF_COLOR = sf::Color(66, 71, 74);
	bool m_IsOn = false;
	const float ANCHOR_RAD = 5.f;
	sf::CircleShape m_Anchor;
};