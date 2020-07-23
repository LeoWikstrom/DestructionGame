#include "Enemy.h"
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
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt, sf::RenderWindow * window)
{
	m_KeyFrameDuration += dt;
	if (m_KeyFrameDuration > m_AnimationSpeed)
	{
		m_pCurrentKeyFrame->x  = (m_pCurrentKeyFrame->x + 1) % m_pSpriteSheetSize->x;
		m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

		m_KeyFrameDuration = 0;
	}
}

void Enemy::Render(sf::RenderWindow * window)
{
	window->draw(*m_pSprite);
}
