#include "Entity.h"
#include "Config.h"
#include <SFML/Graphics.hpp>

Entity::Entity(const char * texturePath) : m_pTex(new sf::Texture), m_pSprite(new sf::Sprite), m_pCurrentKeyFrame(new sf::Vector2i), m_pKeyFrameSize(new sf::Vector2i), m_pSpriteSheetSize(new sf::Vector2i), m_pStartPosition(new sf::Vector2f)
{
	m_pTex->loadFromFile(texturePath);
	m_pSprite->setTexture(*m_pTex);
	m_pSprite->setScale(SCALE, SCALE);
	m_BoundOffsetX = 1;
	m_BoundOffsetY = 1;
	m_OffsetBounds = 0;
	m_IsOffset = false;

	UpdateBounds();
}

Entity::~Entity()
{
	delete m_pTex;
	delete m_pSprite;
	delete m_pCurrentKeyFrame;
	delete m_pKeyFrameSize;
	delete m_pSpriteSheetSize;
	delete m_pStartPosition;
}

void Entity::Move(float dt)
{
	m_pSprite->move(m_SpeedX * dt, m_SpeedY * dt);
	UpdateBounds();
}

bool Entity::CheckTerrainCollision(sf::Image * terrain)
{
	return false;
}

const sf::Vector2f Entity::GetPosition()
{
	return m_pSprite->getPosition();
}

void Entity::SetPosition(int x, int y)
{
	m_pSprite->setPosition(x, y);
	UpdateBounds();
}

void Entity::SetPosition(sf::Vector2f position)
{
	m_pSprite->setPosition(position);
	UpdateBounds();
}

void Entity::UpdateBounds()
{
	m_LeftBound = ((int)m_pSprite->getGlobalBounds().left + m_BoundOffsetX * SCALE + m_OffsetBounds);// % (Config::GetInstance().GetWindowSizeWidth() * 2);
	m_RightBound = ((int)m_pSprite->getGlobalBounds().left + (int)m_pSprite->getGlobalBounds().width - m_BoundOffsetX * SCALE + m_OffsetBounds);// % (Config::GetInstance().GetWindowSizeWidth() * 2);
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + m_BoundOffsetY * SCALE;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pSprite->getGlobalBounds().height - m_BoundOffsetY * SCALE;
}

void Entity::OffsetBounds()
{
	m_OffsetBounds -= Config::GetInstance().GetWindowSizeWidth();
	UpdateBounds();
}

