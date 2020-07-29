#include "Enemy.h"
#include "Projectile.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Enemy::Enemy(const char* texturePath, const char* weaponTexturePath, int detectionRadius, int accuracy) : Character(texturePath, weaponTexturePath, 1)
{
	m_pKeyFrameSize->x = 16;
	m_pKeyFrameSize->y = 16;
	m_pSpriteSheetSize->x = 6;
	m_pSpriteSheetSize->y = 1;
	m_BaseKeyFrame = 0;
	m_AnimationStart = 0;
	m_AnimationLength = 4;
	m_pCurrentKeyFrame->x = m_AnimationStart;
	m_pCurrentKeyFrame->y = 0;
	m_pStartPosition->x = Config::GetInstance().GetWindowSizeWidth() - 16;
	m_pStartPosition->y = 200 * SCALE;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.f;
	m_BoundOffsetX = 2;
	m_BoundOffsetY = 1;

	m_pSprite->setTextureRect(sf::IntRect(m_AnimationStart, 0, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	SetPosition(*m_pStartPosition);
	m_pSprite->setScale(SCALE, SCALE);

	m_WalkingSpeed = -20 * SCALE;
	m_SpeedX = -20 * SCALE;
	m_SpeedY = 0;

	m_DetectionRadius = detectionRadius * SCALE;
	m_Accuracy = accuracy * SCALE;

	m_pWeaponSprite->setTextureRect(sf::IntRect(0, 0, m_pWeaponSprite->getGlobalBounds().width, m_pWeaponSprite->getGlobalBounds().height));
	m_pWeaponSprite->setOrigin(sf::Vector2f(m_pWeaponSprite->getGlobalBounds().width / 2, m_pWeaponSprite->getGlobalBounds().height / 2));
	m_pWeaponSprite->setPosition(sf::Vector2f(m_pStartPosition->x + 1 * SCALE, m_pStartPosition->y + 7 * SCALE));
	m_pWeaponSprite->setScale(SCALE, SCALE);
	m_WeaponAngle = 0;

	m_Projectiles.front()->SetDirection(-1);

	m_Jumping = false;


	UpdateBounds();
}

Enemy::~Enemy()
{
}

void Enemy::CheckForPlayer(int playerX, int playerY)
{
	if (sqrt(pow(m_pSprite->getPosition().x - playerX, 2) + pow(m_pSprite->getPosition().y - playerY, 2)) <= m_DetectionRadius && m_pSprite->getPosition().x > playerX)
	{
		m_WalkingSpeed = 0;
		AimForPlayer(playerX, playerY);
	}
	else
	{
		m_WalkingSpeed = -20 * SCALE;
	}
}

void Enemy::AimForPlayer(int playerX, int playerY)
{
	m_pSprite->setTextureRect(sf::IntRect(2 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	if (!IsInAir() && !m_Projectiles.front()->IsShooting())
	{
		//Shoot();
	}
}

void Enemy::Update(float dt, sf::RenderWindow * window)
{
	Move(dt);

	if (!m_Projectiles.front()->IsShooting())
	{
		m_Projectiles.front()->SetPosition(m_pWeaponSprite->getPosition().x, m_pWeaponSprite->getPosition().y - 5 * SCALE);
	}
	else
	{
		m_Projectiles.front()->Update(dt, window);
	}
	if (m_Projectiles.front()->IsExplosion())
	{
		m_Projectiles.front()->UpdateExplosion(dt);
	}

	if ((m_SpeedX != 0 || m_SpeedY == 0) && m_WalkingSpeed != 0)
	{
		m_KeyFrameDuration += dt;
		if (m_KeyFrameDuration > m_AnimationSpeed)
		{
			m_pCurrentKeyFrame->x = ((m_pCurrentKeyFrame->x - m_AnimationStart) + 1) % m_AnimationLength + m_AnimationStart;
			m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

			m_KeyFrameDuration = 0;
		}
	}

	if (m_pSprite->getPosition().x <= -16 * SCALE)
	{
		//m_pSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pSprite->getPosition().y);
	}
	if (m_pSprite->getPosition().x >= Config::GetInstance().GetWindowSizeWidth() - 16 * SCALE && m_SpeedX > 0)
	{
		m_SpeedX *= -1;
	}
	if (m_pSprite->getPosition().y <= 0)
	{
		m_SpeedY *= -1;
	}

	if (m_pWeaponSprite->getPosition().x <= -16 * SCALE)
	{
		//m_pWeaponSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pWeaponSprite->getPosition().y);
	}

	UpdateBounds();
}

void Enemy::Render(sf::RenderWindow * window)
{
	m_Projectiles.front()->Render(window);
	if (m_WalkingSpeed == 0)
	{
		window->draw(*m_pWeaponSprite);
	}

	window->draw(*m_pSprite);
}
