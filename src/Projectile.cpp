#include "Projectile.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Explosion.h"

Projectile::Projectile(const char * texturePath, std::vector<Explosion*>* explosions, int damage, int power) : Entity(texturePath), m_pExplosions(explosions), m_pTerrainImage(nullptr)
{
	m_Damage = damage;
	m_Power = power;
	m_Shooting = false;
	m_SpeedX = 0;
	m_SpeedY = 0;
	m_Direction = 1;

	m_pKeyFrameSize->x = 14;
	m_pKeyFrameSize->y = 9;
	m_pSpriteSheetSize->x = 1;
	m_pSpriteSheetSize->y = 2;
	m_BaseKeyFrame = 0;
	m_AnimationLength = 0;
	m_AnimationStart = 0;
	m_pCurrentKeyFrame->x = m_AnimationStart;
	m_pCurrentKeyFrame->y = 0;
	m_pStartPosition->x = 100 * SCALE;
	m_pStartPosition->y = 200 * SCALE;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.15f;

	m_BoundOffsetX = 2;
	m_BoundOffsetY = 2;

	m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	SetPosition(*m_pStartPosition);
	m_pSprite->setOrigin(m_pKeyFrameSize->x / 2, 0);
}

Projectile::~Projectile()
{
	//while (!m_Explosions.empty())
	//{
	//	delete m_Explosions.back();
	//	m_Explosions.pop_back();
	//}
}

void Projectile::SetRotation(float angle)
{
	m_pSprite->setRotation(angle);
}

void Projectile::SetDirection(int direction)
{
	m_Direction = direction;
}

void Projectile::Shoot()
{
	if (!m_Shooting)
	{
		m_Shooting = true;

		m_SpeedX = m_Direction * m_Power * cos(m_pSprite->getRotation() * 3.141592654f / 180);
		m_SpeedY = m_Direction * m_Power * sin(m_pSprite->getRotation() * 3.141592654f / 180);

		UpdateBounds();
	}
}

bool Projectile::IsShooting()
{
	return m_Shooting;
}

void Projectile::Move(float dt)
{
	m_SpeedY += 9.81 * 20 * SCALE * dt;
	Entity::Move(dt);
}

bool Projectile::CheckTerrainCollision(sf::Image * terrain)
{
	m_pTerrainImage = terrain;

	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight() && m_BottomBound >= 0)
	{
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, std::max(m_TopBound, 0)) == GROUND_COLOUR)
			{
				m_Shooting = false;
				m_pExplosions->push_back(new Explosion(m_pSprite->getPosition().x + m_pSprite->getGlobalBounds().width / 2, m_pSprite->getPosition().y + m_pSprite->getGlobalBounds().height / 2, m_Damage));
				return true;
			}
		}
	}

	return false;
}

void Projectile::Update(float dt, sf::RenderWindow * window, float offset)
{
	sf::Vector2f pos1 = m_pSprite->getPosition();
	Move(dt);
	sf::Vector2f pos2 = m_pSprite->getPosition();

	float angle = atan2(pos2.y - pos1.y, pos2.x - pos1.x) * 180 / 3.141592654f;

	if (m_Direction == 1)
	{
		m_pCurrentKeyFrame->y = 0;
		m_pSprite->setRotation(angle);
	}
	else
	{
		m_pCurrentKeyFrame->y = 1;
		m_pSprite->setRotation(180 + angle);
	}
	m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

	if (m_pSprite->getPosition().y > Config::GetInstance().GetWindowSizeHeight() || m_RightBound < (int)(offset - (Config::GetInstance().GetWindowSizeWidth() / 2)) + m_OffsetBounds || m_LeftBound > (int)(offset + (Config::GetInstance().GetWindowSizeWidth() * 3 / 2)) + m_OffsetBounds - m_Damage)
	{
		m_Shooting = false;
	}

	UpdateBounds();

}

void Projectile::Render(sf::RenderWindow * window)
{
	if (m_Shooting)
	{
		window->draw(*m_pSprite);
	}
}
