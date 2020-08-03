#include "Explosion.h"
#include <SFML/Graphics.hpp>
#include "Character.h"
#include "Config.h"
#include "Terrain.h"
Explosion::Explosion(float x, float y, float radius):m_Rad(radius), m_Ttl(1.0f), m_pTex(new sf::Texture()), m_pSpr(new sf::Sprite()), m_Exploded(false)
{
	m_pTex->loadFromFile("..\\resources\\explosion.png");
	m_pSpr->setTexture(*m_pTex);
	float scaling = m_Rad / (m_pSpr->getGlobalBounds().width/ 2);
	m_pSpr->setScale(scaling, scaling);
	m_pSpr->setPosition(x - m_Rad, y - m_Rad);

	m_New = true;
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
	m_New = false;
}

void Explosion::Render(sf::RenderWindow* window)
{
	window->draw(*m_pSpr);
}

bool Explosion::CheckCollision(Character* character)
{
	bool res = false;
	if (m_New)
	{
		float x = m_pSpr->getPosition().x + m_Rad;
		float y = m_pSpr->getPosition().y + m_Rad;

		float centerDiffX = character->GetPositionMiddle().x - x;
		float centerDiffY = character->GetPositionMiddle().y - y;
		float leftBound = character->GetHorizontalBounds().x;
		float rightBound = character->GetHorizontalBounds().y;
		float topBound = character->GetVerticalBounds().x;
		float bottomBound = character->GetVerticalBounds().y;

		float xDiff = abs(x - leftBound);
		xDiff = std::min(xDiff, abs(x - rightBound));
		float yDiff = abs(y - topBound);
		yDiff = std::min(yDiff, y - bottomBound);

		if (abs(xDiff) < m_Rad && (yDiff) < m_Rad)
		{
			float angle = atan2(abs(centerDiffY), abs(centerDiffX));
			float mag = 10000 * SCALE / sqrt(pow(centerDiffX, 2) + pow(centerDiffY, 2));

			float explodeX = mag * cos(angle) * centerDiffX / abs(centerDiffX);
			float explodeY = mag * sin(angle) * centerDiffY / abs(centerDiffY);

			character->Explode((sf::Vector2f(explodeX, explodeY)));
			res = true;
		}
	}

	return res;
}

bool Explosion::ShouldLive()
{
	return m_Ttl > 0.0f;
}

void Explosion::ClearTerrain(sf::Image* terrain, float offset)
{
	if (!m_Exploded)
	{
		sf::CircleShape explosion;
		sf::RenderTexture renderTex;
		sf::Texture texture;
		sf::Sprite sprite;

		texture.loadFromImage(*terrain);
		sprite.setTexture(texture);
		renderTex.create(Config::GetInstance().GetWindowSizeWidth() * 2, Config::GetInstance().GetWindowSizeHeight());
		renderTex.clear();
		renderTex.draw(sprite);

		explosion.setRadius(m_Rad);
		explosion.setFillColor(SKY_COLOUR);
		explosion.setPosition(m_pSpr->getPosition() + sf::Vector2f(offset, 0));
		renderTex.draw(explosion);

		renderTex.display();

		*terrain = renderTex.getTexture().copyToImage();
	}
}
