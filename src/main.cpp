#include <SFML/Graphics.hpp>
#include<iostream>

static const float cMoveOnPressX = 1. / 15.f;
static const float cMoveOnPressY = 1. / 5.f;
static const float cBottomLevelY = 700.f;
static const float cGravityY = 0.0001f;

bool doCollide(sf::Shape& shape1, sf::Shape& shape2)
{
	sf::FloatRect rect1 = shape1.getGlobalBounds();
	sf::FloatRect rect2 = shape2.getGlobalBounds();
	return rect1.findIntersection(rect2).has_value();
}

class Platform
{

};

class Player
{
public:
	Player() : m_rectangle({ 30.f, 100.f }) 
	{
		m_rectangle.setPosition({ 50.f, 0.f });
		m_rectangle.setFillColor(sf::Color::Cyan);
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

		/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			if (m_moveVector.y >= 0.0)
				m_moveVector.y = -cMoveOnPressY;
		}*/
		if (/*m_moveVector.y < 0.0 || m_rectangle.getPosition().y < cBottomLevelY*//*m_moveVector.y < 0.0 ||*/ !onGround /* || j*/)
		{
			m_moveVector.y += cGravityY;
			//j = false;
		} 
		return m_moveVector;
	}

	void jamp()
	{
		if (onGround)
		{
			m_moveVector.y = -cMoveOnPressY;
			onGround = false;
			//j = true;
		}

	}

	void interactionPlatform(sf::RectangleShape Platform)
	{
		if (doCollide(m_rectangle, Platform))
		{
			onGround = true;
			if(m_moveVector.y > 0.0) m_moveVector.y = 0;
		}
		else onGround = false;

	}

	void move()
	{
		m_rectangle.move(m_moveVector);
	}

private:
	sf::RectangleShape m_rectangle;
	// we didn't decided what is it
	sf::Vector2f m_moveVector = {0.0, 0.0};
	bool j = 0;
	bool onGround = false;
	/* {
		return m_rectangle.getPosition().y >= cBottomLevelY;
	}*/


};

int main()
{
	sf::RenderWindow window( sf::VideoMode( { 1600, 800 } ), "ball" );
	sf::CircleShape circle( 50.f );
	sf::RectangleShape rectangle({30.f, 100.f});
	circle.setPosition({ 50.f, 700.f });
	circle.setFillColor( sf::Color::Green );
	rectangle.setFillColor(sf::Color::Red);
	rectangle.setPosition({ 500.f, 700.f });
	sf::RectangleShape platform({ 600.f, 30.f });
	platform.setFillColor(sf::Color::Yellow);
	platform.setPosition({ 50.f, 770.f });
	Player player;

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
					player.jamp();
				}
			}
		}
		player.interactionPlatform(platform);
		/*if (doCollide(circle, rectangle))
		{
			circle.setFillColor(sf::Color::Cyan);
		}
		else
		{
			circle.setFillColor(sf::Color::Green);
		}*/
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			circle.move({ 1/15.f, 0.f });
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			circle.move({ -1/15.f, 0.f });
		}

		sf::Vector2f& playerMove = player.calculateMoveVector();
		player.move();

		window.clear();
		//window.draw(circle);
		window.draw(player.getShape());
		//window.draw( rectangle );
		window.draw(platform);
		window.display();
	}
}


