#include "Enemy.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Enemy::Enemy(const char * texturePath) : Entity(texturePath)
{
	m_pKeyFrameSize->x = 16;
	m_pKeyFrameSize->y = 16;
	m_pSpriteSheetSize->x = 4;
	m_pSpriteSheetSize->y = 1;
	m_pCurrentKeyFrame->x = 0;
	m_pCurrentKeyFrame->y = 0;
	m_pStartPosition->x = 400;
	m_pStartPosition->y = 200;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.f;

	m_pSprite->setTextureRect(sf::IntRect(0, 0, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pSprite->setPosition(*m_pStartPosition);

	m_WalkingSpeed = -20;
	m_SpeedX = -20;
	m_SpeedY = 100;
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt, sf::RenderWindow * window)
{
	Move(dt);

	if (m_SpeedX != 0 || m_SpeedY == 0)
	{
		m_KeyFrameDuration += dt;
		if (m_KeyFrameDuration > m_AnimationSpeed)
		{
			m_pCurrentKeyFrame->x = (m_pCurrentKeyFrame->x + 1) % m_pSpriteSheetSize->x;
			m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

			m_KeyFrameDuration = 0;
		}
	}

	if (m_pSprite->getPosition().x <= -16)
	{
		m_pSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pSprite->getPosition().y);
	}

	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 2;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pSprite->getGlobalBounds().width - 2;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 1;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pSprite->getGlobalBounds().height - 1;

}

void Enemy::Render(sf::RenderWindow * window)
{
	window->draw(*m_pSprite);
}
