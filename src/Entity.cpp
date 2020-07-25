#include "Entity.h"
#include "Config.h"
#include <SFML/Graphics.hpp>

Entity::Entity(const char * texturePath, const char * weaponTexturePath) : m_pTex(new sf::Texture), m_pSprite(new sf::Sprite), m_pWeaponTex(new sf::Texture), m_pWeaponSprite(new sf::Sprite), m_pCurrentKeyFrame(new sf::Vector2i), m_pKeyFrameSize(new sf::Vector2i), m_pSpriteSheetSize(new sf::Vector2i), m_pStartPosition(new sf::Vector2f)
{
	m_pTex->loadFromFile(texturePath);
	m_pSprite->setTexture(*m_pTex);
	m_pWeaponTex->loadFromFile(weaponTexturePath);
	m_pWeaponSprite->setTexture(*m_pWeaponTex);
	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 1;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pSprite->getGlobalBounds().width - 1;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 1;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pSprite->getGlobalBounds().height - 1;
	
	m_Falling = false;
	m_FallingTime = 0;
}

Entity::~Entity()
{
	delete m_pTex;
	delete m_pSprite;
	delete m_pWeaponTex;
	delete m_pWeaponSprite;
	delete m_pCurrentKeyFrame;
	delete m_pKeyFrameSize;
	delete m_pSpriteSheetSize;
	delete m_pStartPosition;
}

void Entity::Move(float dt)
{
	if (m_Falling)
	{
		m_SpeedY += 9.81*20*dt;
		m_Jumping = false;
		m_FallingTime += dt;
	}
	m_pSprite->move(m_SpeedX * dt, m_SpeedY * dt);
	m_pWeaponSprite->move(m_SpeedX * dt, m_SpeedY * dt);
}

void Entity::CheckTerrainCollision(sf::Image * terrain)
{
	m_Falling = true;
	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight())
	{
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, m_BottomBound) == GROUND_COLOUR)
			{
				if (m_SpeedY >= 0)
				{
					if (!m_Jumping)
					{
						m_SpeedY = 0;
					}
					m_Falling = false;
					m_FallingTime = 0;
					m_SpeedX = m_WalkingSpeed;
				}

				bool walkableRight = true, walkableLeft = true, fullStop = false;
				int height = 0;

				for (int j = m_BottomBound; j >= m_BottomBound - 3; --j)
				{
					if (m_SpeedX < 0)
					{
						if (terrain->getPixel(m_LeftBound, j) == SKY_COLOUR)
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
						if (terrain->getPixel(m_RightBound, j) == SKY_COLOUR)
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

				for (int j = m_BottomBound - 4; j >= m_TopBound; --j)
				{
					if (m_SpeedX < 0)
					{
						if (terrain->getPixel(m_LeftBound, j) == GROUND_COLOUR)
						{
							walkableLeft = false;
							fullStop = true;
						}
					}
					else if (m_SpeedX > 0)
					{
						if (terrain->getPixel(m_RightBound, j) == GROUND_COLOUR)
						{
							walkableRight = false;
							fullStop = true;
						}
					}
				}

				if ((walkableLeft && m_SpeedX < 0 || walkableRight && m_SpeedX > 0) && !m_Jumping && !m_Falling && !fullStop)
				{
					m_SpeedY = -20 * height;
				}
				else if ((!m_Jumping && !m_Falling) || (m_Jumping && IsInAir()) || (m_Falling && IsInAir()) || fullStop)
				{
					m_pSprite->move(((m_SpeedX < 0) * 2 - 1) * (m_SpeedX != 0), 0);
					m_pWeaponSprite->move(((m_SpeedX < 0) * 2 - 1) * (m_SpeedX != 0), 0);
					m_SpeedX = 0;
				}
				break;
			}

			if (terrain->getPixel(i, m_TopBound) == GROUND_COLOUR)
			{
				if (m_SpeedY < 0)
				{
					m_SpeedY = 0;
				}
				break;
			}
		}
	}
}

void Entity::RotateWeapon(bool direction)
{
	if ((m_WeaponAngle >= 0 && m_WeaponAngle < 45) || (m_WeaponAngle <= 360 && m_WeaponAngle > 315) || (m_WeaponAngle == 45 && !direction) || (m_WeaponAngle == 315 && direction))
	{
		if (m_pCurrentKeyFrame->y == 0)
		{
			m_pWeaponSprite->rotate(5.625f * (2 * direction - 1));
			m_WeaponAngle = m_pWeaponSprite->getRotation();
		}
		else
		{
			m_pWeaponSprite->rotate(5.625f * -(2 * direction - 1));
			m_WeaponAngle = (360 - m_pWeaponSprite->getRotation());
		}
	}
}

void Entity::SetWeaponRotation(float rotation)
{
	m_pWeaponSprite->setRotation(rotation);
	m_WeaponAngle = m_pWeaponSprite->getRotation();
}

const sf::Vector2f Entity::GetPosition()
{
	return m_pSprite->getPosition();
}

bool Entity::IsInAir()
{
	return m_FallingTime > 0.5f;
}

