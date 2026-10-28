#include <SFML/Graphics.hpp>
#include<iostream>
#include <memory>
#include <vector>

static const float cMoveOnPressX = 1. / 15.f;
static const float cMoveOnPressY = 1. / 5.f;
static const float cBottomLevelY = 700.f;
static const float cGravityY = 0.0001f;

bool doCollide(const sf::Shape& shape1, const sf::Shape& shape2)
{
	sf::FloatRect rect1 = shape1.getGlobalBounds();
	sf::FloatRect rect2 = shape2.getGlobalBounds();
	return rect1.findIntersection(rect2).has_value();
}

class BaseObject {
protected:
	BaseObject() = default;
	BaseObject(const BaseObject&) = delete;
	BaseObject(const BaseObject&&) = delete;
	BaseObject& operator=(const BaseObject&) = delete;
};

class Renderer {
public:
private:
	std::vector<std::shared_ptr<BaseObject>> m_objectsToRender;
};

class Platform : public BaseObject
{
protected:
	sf::RectangleShape m_rectangle;

public:
	Platform(const sf::Vector2f& iSize, const sf::Vector2f& iPosition)
	{
		m_rectangle.setSize(iSize);
		m_rectangle.setPosition(iPosition);
		m_rectangle.setFillColor(sf::Color::Green);
	}
	const sf::RectangleShape& getShape() const { return m_rectangle; }
};

class Player : public BaseObject
{
private:
	sf::RectangleShape m_rectangle;
	// we didn't decided what is it
	sf::Vector2f m_moveVector = { 0.0, 0.0 };
	bool j = 0;
	bool m_onGround = false;

	Player() : m_rectangle({ 30.f, 100.f }) 
	{
		m_rectangle.setPosition({ 50.f, 0.f });
		m_rectangle.setFillColor(sf::Color::Cyan);
	}

public:
	static std::shared_ptr<Player> create()
	{
		return std::shared_ptr<Player>(new Player());
	}

	const sf::RectangleShape& getShape() const { return m_rectangle; }
	sf::Vector2f& calculateMoveVector()
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
			m_moveVector.x = cMoveOnPressX;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
			m_moveVector.x = -cMoveOnPressX;
		else
		{
			m_moveVector.x = 0.0;
		}

		if (!m_onGround)
		{
			m_moveVector.y += cGravityY;
		} 
		return m_moveVector;
	}

	void jump()
	{
		if (m_onGround)
		{
			m_moveVector.y = -cMoveOnPressY;
			m_onGround = false;
		}

	}

	void interactionPlatform(Platform* ipPlatform)
	{
		if (ipPlatform == nullptr)
			return;
		if (doCollide(m_rectangle, ipPlatform->getShape()))
		{
			m_onGround = true;
			if(m_moveVector.y > 0.0) m_moveVector.y = 0;
		}
		else m_onGround = false;

	}

	void move()
	{
		m_rectangle.move(m_moveVector);
	}

};

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1600, 800 } ), "ball" );
	// frame rate. should be 240 or 120
	window.setFramerateLimit(1000);
	std::unique_ptr<sf::CircleShape> pCircle(new sf::CircleShape(100.f));
	Platform platform1({ 600.f, 30.f }, { 50.f, 770.f });
	Platform platform2({ 60.f, 30.f }, { 300.f, 670.f });
	std::shared_ptr<Player> player = Player::create();

	while ( window.isOpen() )
	{
		while ( const std::optional event = window.pollEvent() )
		{
			if ( event->is<sf::Event::Closed>() )
				window.close();
			if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{ 
				if (keyPressed->code == sf::Keyboard::Key::Space)
				{
					player->jump();
				}
			}
		}
		player->interactionPlatform(&platform1);
		player->interactionPlatform(&platform2);
		
		sf::Vector2f& playerMove = player->calculateMoveVector();
		player->move();

		window.clear();
		window.draw(player->getShape());
		window.draw(platform1.getShape());
		window.draw(platform2.getShape());
		window.display();
	}
}


