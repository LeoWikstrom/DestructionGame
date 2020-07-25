#include "Enemy.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Enemy::Enemy(const char* texturePath, const char* weaponTexturePath, int detectionRadius, int accuracy) : Character(texturePath, weaponTexturePath)
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
	m_pStartPosition->x = 400;
	m_pStartPosition->y = 200;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.f;

	m_pSprite->setTextureRect(sf::IntRect(m_AnimationStart, 0, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pSprite->setPosition(*m_pStartPosition);

	m_WalkingSpeed = -20;
	m_SpeedX = -20;
	m_SpeedY = 150;

	m_DetectionRadius = detectionRadius;
	m_Accuracy = accuracy;

	m_pWeaponSprite->setTextureRect(sf::IntRect(0, 0, m_pWeaponSprite->getGlobalBounds().width, m_pWeaponSprite->getGlobalBounds().height));
	m_pWeaponSprite->setOrigin(sf::Vector2f(m_pWeaponSprite->getGlobalBounds().width / 2, m_pWeaponSprite->getGlobalBounds().height / 2));
	m_pWeaponSprite->setPosition(sf::Vector2f(m_pStartPosition->x + 1, m_pStartPosition->y + 7));
	m_WeaponAngle = 0;

	m_Jumping = false;
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
		m_WalkingSpeed = -20;
	}
}

void Enemy::AimForPlayer(int playerX, int playerY)
{
	m_pSprite->setTextureRect(sf::IntRect(2 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	
}

void Enemy::Update(float dt, sf::RenderWindow * window)
{
	Move(dt);

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

	if (m_pSprite->getPosition().x <= -16)
	{
		m_pSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pSprite->getPosition().y);
	}

	if (m_pWeaponSprite->getPosition().x <= -16)
	{
		m_pWeaponSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pWeaponSprite->getPosition().y);
	}

	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 2;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pSprite->getGlobalBounds().width - 2;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 1;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pSprite->getGlobalBounds().height - 1;
}

void Enemy::Render(sf::RenderWindow * window)
{
	if (m_WalkingSpeed == 0)
	{
		window->draw(*m_pWeaponSprite);
	}

	window->draw(*m_pSprite);
}
