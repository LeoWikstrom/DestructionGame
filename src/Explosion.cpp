#include "Explosion.h"
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Config.h"
Explosion::Explosion(float x, float y, float radius):m_Rad(radius), m_Ttl(1.0f), m_pTex(new sf::Texture()), m_pSpr(new sf::Sprite())
{
	m_pTex->loadFromFile("..\\resources\\explosion.png");
	m_pSpr->setTexture(*m_pTex);
	float scaling = m_Rad / (m_pSpr->getGlobalBounds().width/ 2);
	m_pSpr->setScale(scaling, scaling);
	m_pSpr->setOrigin(m_pSpr->getGlobalBounds().width / 2, m_pSpr->getGlobalBounds().height / 2);
	m_pSpr->setPosition(x - m_pSpr->getGlobalBounds().width / 2, y - m_pSpr->getGlobalBounds().height / 2);
}

Explosion::~Explosion()
{
	delete m_pTex;
	delete m_pSpr;
}

void Explosion::Update(float dt, sf::RenderWindow* window)
{
	m_Ttl -= dt;
	m_Ttl = std::max(m_Ttl, 0.0f);
	m_pSpr->setColor(sf::Color(m_pSpr->getColor().r, m_pSpr->getColor().g, m_pSpr->getColor().b, (m_Ttl - 1) * 255));
}

void Explosion::Render(sf::RenderWindow* window)
{
	window->draw(*m_pSpr);
}

bool Explosion::CheckCollision(Character* character)
{
	bool res = false;
	float x = m_pSpr->getPosition().x + m_pSpr->getGlobalBounds().width / 2;
	float y = m_pSpr->getPosition().y + m_pSpr->getGlobalBounds().height / 2;

	float xDiff = x - character->GetPositionMiddle().x;
	float yDiff = y - character->GetPositionMiddle().y;

	if (abs(xDiff) < m_Rad && (yDiff) < m_Rad)
	{
		character->Explode((sf::Vector2f(300 / xDiff, 300 / yDiff)));
		res = true;
	}

	return res;
}

bool Explosion::ShouldLive()
{
	return m_Ttl > 0.0f;
}
