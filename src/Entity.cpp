#include "Entity.h"
#include <SFML/Graphics.hpp>

Entity::Entity(const char * texturePath) : m_pTex(new sf::Texture), m_pSprite(new sf::Sprite), m_pCurrentKeyFrame(new sf::Vector2i), m_pKeyFrameSize(new sf::Vector2i), m_pSpriteSheetSize(new sf::Vector2i), m_pStartPosition(new sf::Vector2f)
{
	m_pTex->loadFromFile(texturePath);
	m_pSprite->setTexture(*m_pTex);
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

