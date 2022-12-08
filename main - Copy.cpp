#include <SFML/Graphics.hpp>
#include <iostream>


template<class T>
class DraggableComponent
{
public:
	[[nodiscard]] virtual const sf::Vector2f& GetPosition() const = 0;
	virtual void SetOutlineColor(sf::Color color) = 0;
	[[nodiscard]] virtual sf::FloatRect GetBounds() const = 0;
	virtual void SetPosition(const sf::Vector2f& pos) = 0;

private:
};


class DrawableOperator : public sf::Drawable, public DraggableComponent
{
public:
	DrawableOperator()
	{
		m_Box.setSize({BOX_W, BOX_W});
		m_Box.setFillColor({13, 98, 140});
		m_Box.setOrigin(BOX_W/2, BOX_W/2);
		m_Box.setOutlineThickness(-5);
		m_Box.setPosition(500, 300);
	}

	const sf::Vector2f& GetPosition() const override { return m_Box.getPosition(); }
	sf::FloatRect GetBounds() const override { return m_Box.getGlobalBounds(); }
	void SetPosition(const sf::Vector2f& pos) override { m_Box.setPosition(pos); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_Box, states);
	}

public:
	const float BOX_W = 100.f;
	const sf::Color UNDEF_OUTLINE = sf::Color::Red;
	const sf::Color VALID_OUTLINE = sf::Color::Green;

private:
	sf::RectangleShape m_Box;
};

class Operator : public DrawableOperator
{
public:
	Operator()
	{
		SetOutlineColor(GetOutlineColor());
	}
	[[nodiscard]] bool IsConnected() const { return in1 != -1 && in2 != -1; }
	void SetOutlineColor(sf::Color color) override { m_Box.setOutlineColor(color); }
	sf::Color GetOutlineColor() const { return IsConnected() ? VALID_OUTLINE : UNDEF_OUTLINE; }

	int in1 = -1;
	int in2 = -1;
};

class Switch : public sf::Drawable
{
public:
	Switch()
	{
		m_Circ.setPosition(100.f, 100.f);
		m_Circ.setRadius(RAD);
		m_Circ.setOrigin(RAD/2, RAD/2);
		m_Circ.setOutlineThickness(-3);
		m_Circ.setOutlineColor({204, 47, 144});
		UpdateColor();
	}

	void Toggle() { m_IsOn = !m_IsOn; UpdateColor(); }
	void UpdateColor() {
		if (m_IsOn)
			m_Circ.setFillColor(OnColor);
		else
			m_Circ.setFillColor(OffColor);
	}

	const sf::Vector2f& GetPosition() const { return m_Circ.getPosition(); }
	void SetOutlineColor(sf::Color color) { m_Circ.setOutlineColor(color); }
	sf::FloatRect GetBounds() const { return m_Circ.getGlobalBounds(); }
	void SetPosition(const sf::Vector2f& pos) { m_Circ.setPosition(pos); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(m_Circ, states);
	}

private:
	const float RAD = 30.f;
	const sf::Color OnColor = sf::Color::Green;
	const sf::Color OffColor = sf::Color(66, 71, 74);
	bool m_IsOn = false;
	sf::CircleShape m_Circ;
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Fuzzycs!");
	Operator op;
	Switch sw;
	std::vector<DrawableOperator

	Operator* dragging = nullptr;
	sf::Vector2f draggingOffset;

    while (window.isOpen())
    {
		sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
		sf::Vector2f mousePos = {(float) mousePosInt.x, (float) mousePosInt.y};

        sf::Event event{};
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (!dragging && event.type == sf::Event::MouseButtonPressed && op.GetBounds().contains(mousePos))
			{
				draggingOffset = op.GetPosition() - mousePos;
				dragging = &op;
			}
			else if (dragging && event.type == sf::Event::MouseButtonReleased)
			{
				dragging = nullptr;
			}
			else if (event.type == sf::Event::MouseButtonPressed && sw.GetBounds().contains(mousePos))
			{
				sw.Toggle();
			}
        }

		if (dragging)
				dragging->SetPosition(mousePos + draggingOffset);

        window.clear();
        window.draw(op);
		window.draw(sw);
        window.display();
    }

    return 0;
}