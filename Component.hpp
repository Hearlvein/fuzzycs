#pragma once

#include <SFML/Graphics.hpp>

typedef sf::Vector2f v2;

class Component : public sf::Drawable
{
public:
	Component() = default;
	~Component() override = default;

	[[nodiscard]] const v2& GetPosition() const { return m_Shape->getPosition(); }
	[[nodiscard]] sf::FloatRect GetBounds() const { return m_Shape->getGlobalBounds(); }

	virtual void SetPosition(const v2& pos) { m_Shape->setPosition(pos); }
	void SetOutlineColor(sf::Color color) { m_Shape->setOutlineColor(color); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(*m_Shape, states); }

protected:
	std::unique_ptr<sf::Shape> m_Shape = nullptr;
};