#include "Entity.h"
#include "Config.h"
#include <SFML/Graphics.hpp>

Entity::Entity(const char * texturePath) : m_pTex(new sf::Texture), m_pSprite(new sf::Sprite), m_pCurrentKeyFrame(new sf::Vector2i), m_pKeyFrameSize(new sf::Vector2i), m_pSpriteSheetSize(new sf::Vector2i), m_pStartPosition(new sf::Vector2f)
{
	m_pTex->loadFromFile(texturePath);
	m_pSprite->setTexture(*m_pTex);
	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 1;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pSprite->getGlobalBounds().width - 1;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 1;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pSprite->getGlobalBounds().height - 1;
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
}

void Entity::CheckTerrainCollision(sf::Image * terrain)
{
	m_SpeedY = 100;
	m_SpeedX = 0;
	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight())
	{
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, m_BottomBound) == sf::Color::Black)
			{
				m_SpeedY = 0;
				m_SpeedX = m_WalkingSpeed;
				bool walkableRight = true, walkableLeft = true;
				int height = 0;

				for (int j = m_BottomBound; j >= m_BottomBound - 3; --j)
				{
					if (m_SpeedX < 0)
					{
						if (terrain->getPixel(m_LeftBound, j) == sf::Color::White)
						{
							walkableLeft = true;

						}
						else
						{
							walkableLeft = false;
							height++;

						}
					}
					else if (m_SpeedX > 0)
					{
						if (terrain->getPixel(m_RightBound, j) == sf::Color::White)
						{
							walkableRight = true;
						}
						else
						{
							walkableRight = false;
							height++;
						}
					}
				}

				if (walkableLeft && m_SpeedX < 0 || walkableRight && m_SpeedX > 0)
				{
					m_SpeedY = -20 * height;
				}
				else
				{
					m_SpeedX = 0;
				}
				break;
			}
		}
	}
}

