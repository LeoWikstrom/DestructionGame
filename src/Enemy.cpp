#include "Enemy.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Enemy::Enemy(const char* texturePath, const char* weaponTexturePath) : Entity(texturePath, weaponTexturePath)
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
			m_pCurrentKeyFrame->x = ((m_pCurrentKeyFrame->x - m_AnimationStart) + 1) % m_AnimationLength + m_AnimationStart;
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
