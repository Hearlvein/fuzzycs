#include <SFML/Graphics.hpp>
#include <iostream>

typedef sf::Vector2f v2;

class Component : public sf::Drawable
{
public:
	[[nodiscard]] const v2& GetPosition() const { return m_Shape->getPosition(); }
	[[nodiscard]] sf::FloatRect GetBounds() const { return m_Shape->getGlobalBounds(); }
	void SetPosition(const v2& pos) { m_Shape->setPosition(pos); }
	void SetOutlineColor(sf::Color color) { m_Shape->setOutlineColor(color); }
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override { target.draw(*m_Shape, states); }

protected:
	std::unique_ptr<sf::Shape> m_Shape = nullptr;
};

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
		m_Shape->setPosition(500, 300);
		SetOutlineColor(GetOutlineColor());
	}

	[[nodiscard]] sf::Color GetOutlineColor() const { return IsConnected() ? VALID_OUTLINE : UNDEF_OUTLINE; }
	[[nodiscard]] bool IsConnected() const { return in1 != -1 && in2 != -1; }
	[[nodiscard]] v2 GetAnchor1() const { return GetPosition()+anchor1; }
	[[nodiscard]] v2 GetAnchor2() const { return GetPosition()+anchor2; }

private:
	const float BOX_W = 100.f;
	const sf::Color UNDEF_OUTLINE = sf::Color::Red;
	const sf::Color VALID_OUTLINE = sf::Color::Green;

	int in1;
	int in2;
	const v2 anchor1 = {-BOX_W/2, 20};
	const v2 anchor2 = {-BOX_W/2, 80};
};

class Switch : public Component
{
public:
	Switch()
	{
		m_Shape = std::make_unique<sf::CircleShape>();
		auto circ = reinterpret_cast<sf::CircleShape*>(m_Shape.get());
		circ->setRadius(RAD);

		m_Shape->setPosition(700.f, 100.f);
		m_Shape->setOrigin(RAD, RAD);
		m_Shape->setOutlineThickness(-3);
		m_Shape->setOutlineColor({204, 47, 144});
		UpdateColor();
	}

	[[nodiscard]] v2 GetAnchor() const { return GetPosition()+anchor; }
	void Toggle() { m_IsOn = !m_IsOn; UpdateColor(); }
	void UpdateColor() {
		if (m_IsOn)
			m_Shape->setFillColor(ON_COLOR);
		else
			m_Shape->setFillColor(OFF_COLOR);
	}

private:
	const v2 anchor = {0, 0};
	const float RAD = 30.f;
	const sf::Color ON_COLOR = sf::Color::Green;
	const sf::Color OFF_COLOR = sf::Color(66, 71, 74);
	bool m_IsOn = false;
};

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

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 800), "Fuzzycs!");
	window.setFramerateLimit(60);
	Operator op;
	Switch sw;
	Line ln(op.GetAnchor1(), sw.GetAnchor());

	std::vector<Component*> components = {&op, &sw};

	Component* dragging = nullptr;
	v2 draggingOffset;

    while (window.isOpen())
    {
		sf::Vector2i mousePosInt = sf::Mouse::getPosition(window);
		v2 mousePos = {(float) mousePosInt.x, (float) mousePosInt.y};

        sf::Event event{};
        while (window.pollEvent(event))
        {
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (!dragging && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				for (const auto c : components)
				{
					if (c->GetBounds().contains(mousePos))
					{
						draggingOffset = c->GetPosition() - mousePos;
						dragging = c;
					}
				}
			}
			else if (dragging && event.type == sf::Event::MouseButtonReleased)
				dragging = nullptr;
			else if (event.type == sf::Event::MouseButtonPressed && sw.GetBounds().contains(mousePos)
				&& event.mouseButton.button == sf::Mouse::Right)
				sw.Toggle();
        }

		if (dragging)
		{
			dragging->SetPosition(mousePos + draggingOffset);
			ln = Line(op.GetAnchor1(), sw.GetAnchor());
		}

        window.clear();
        for (auto c : components)
			window.draw(*c);
		window.draw(ln);
        window.display();
    }

    return 0;
}