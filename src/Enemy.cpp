#include "Enemy.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Enemy::Enemy(const char * texturePath) : Entity(texturePath)
{
	m_pKeyFrameSize = new sf::Vector2i(16, 16);
	m_pSpriteSheetSize = new sf::Vector2i(4, 1);
	m_pCurrentKeyFrame = new sf::Vector2i(0, 0);
	m_pStartPosition = new sf::Vector2f(400, 200);
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
