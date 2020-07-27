#include "Character.h"
#include "Config.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

Character::Character(const char * texturePath, const char * weaponTexturePath) : Entity(texturePath), m_pWeaponTex(new sf::Texture), m_pWeaponSprite(new sf::Sprite)
{
	m_pWeaponTex->loadFromFile(weaponTexturePath);
	m_pWeaponSprite->setTexture(*m_pWeaponTex);

	m_Falling = false;
	m_FallingTime = 0;

	m_Weight = 1;

	m_Projectiles.push(new Projectile("..\\resources\\projectile.png", 50 * SCALE, 300 * SCALE));

	m_Exploded = false;
}

Character::~Character()
{
	delete m_pWeaponSprite;
	delete m_pWeaponTex;
	while (!m_Projectiles.empty())
	{
		delete m_Projectiles.front();
		m_Projectiles.pop();
	}
}

void Character::Move(float dt)
{
	if (m_Falling)
	{
		m_SpeedY += 9.81 * 20 * SCALE * dt;
		m_Jumping = false;
		m_FallingTime += dt;
	}

	if (abs(m_SpeedX - m_WalkingSpeed) < 0.1)
	{
		m_SpeedX = m_WalkingSpeed;
	}
	else if (m_SpeedX < m_WalkingSpeed)
	{
		m_SpeedX += 50 * SCALE * dt;
	}
	else if (m_SpeedX > m_WalkingSpeed)
	{
		m_SpeedX -= 50 * SCALE * dt;
	}
	m_pSprite->move(m_SpeedX * dt, m_SpeedY * dt);
	m_pWeaponSprite->move(m_SpeedX * dt, m_SpeedY * dt);
	UpdateBounds();

	//if (abs(m_SpeedX) > 0)
	//{
	//	printf("Player position: %f, %f\n", m_pSprite->getPosition().x, m_pSprite->getPosition().y);
	//}
	//	printf("BoundsX: %d, %d\n", m_LeftBound, m_RightBound);
}

bool Character::CheckTerrainCollision(sf::Image * terrain)
{
	m_Falling = true;
	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight())
	{
		int redDown = 0;
		int redUp = 0;
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, m_BottomBound) == GROUND_COLOUR)
			{
				m_Exploded = false;
				if (m_SpeedY >= 0)
				{
					if (!m_Jumping)
					{
						m_SpeedY = 0;
					}
					m_Falling = false;
					m_FallingTime = 0;
				}

				bool walkableRight = true, walkableLeft = true, fullStop = false;
				int height = 0;

				for (int j = m_BottomBound; j >= m_BottomBound - 5 * SCALE; --j)
				{
					if (m_SpeedX < 0)
					{
						if (terrain->getPixel(m_LeftBound, j) != GROUND_COLOUR)
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
						if (terrain->getPixel(m_RightBound, j) != GROUND_COLOUR)
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

				for (int j = m_BottomBound - 5 * SCALE - 1; j >= m_TopBound; --j)
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
				else if (m_SpeedX != 0 && ((!m_Jumping && !m_Falling) || (m_Jumping && IsInAir()) || (m_Falling && IsInAir()) || fullStop))
				{
					m_SpeedX *= -0.25 * SCALE;
				}
				break;
			}

			if (terrain->getPixel(i, m_TopBound) == GROUND_COLOUR)
			{
				if (m_SpeedY < 0)
				{
					m_SpeedY *= -0.1;
				}

				break;
			}


			redDown = std::max(redDown, (int)terrain->getPixel(i, m_BottomBound).r);
			redUp = std::max(redUp, (int)terrain->getPixel(i, m_TopBound).r);
			if (redDown >= 100 && redDown > redUp && (redDown / m_Weight) * SCALE > abs(m_SpeedY))
			{
				m_SpeedY = -(redDown / m_Weight) * SCALE;
				m_WalkingSpeed = 50 * SCALE * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
				m_Exploded = true;
			}
			else if (redUp >= 100 && redUp > redDown && (redUp / m_Weight) * SCALE > abs(m_SpeedY))
			{
				m_SpeedY = (redUp / m_Weight) * SCALE;
				m_WalkingSpeed = 50 * SCALE * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
				m_Exploded = true;
			}
		}

		int redRight = 0;
		int redLeft = 0;
		for (int i = m_TopBound; i <= m_BottomBound; ++i)
		{
			bool fullStop = false;
			if (m_SpeedX < 0)
			{
				if (terrain->getPixel(m_LeftBound, i) == GROUND_COLOUR)
				{
					fullStop = true;
				}
			}
			else if (m_SpeedX > 0)
			{
				if (terrain->getPixel(m_RightBound, i) == GROUND_COLOUR)
				{
					fullStop = true;
				}
			}

			if (IsInAir() && fullStop)
			{
				m_SpeedX *= -0.25;
			}

			redRight = std::max(redRight, (int)terrain->getPixel(m_RightBound, i).r);
			redLeft = std::max(redLeft, (int)terrain->getPixel(m_LeftBound, i).r);
			if (redRight >= 100 && redRight > redLeft && (redRight / m_Weight) * SCALE > abs(m_SpeedX))
			{
				m_SpeedX = -(redRight / m_Weight) * SCALE;
				m_WalkingSpeed = 50 * SCALE * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
				m_Exploded = true;
			}
			else if (redLeft >= 100 && redLeft > redRight && (redLeft / m_Weight) * SCALE > abs(m_SpeedX))
			{
				m_SpeedX = (redLeft / m_Weight) * SCALE;
				m_WalkingSpeed = 50 * SCALE * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
				m_Exploded = true;
			}
		}
	}

	if (m_Projectiles.front()->IsShooting())
	{
		return m_Projectiles.front()->CheckTerrainCollision(terrain);
	}
	else
	{
		return false;
	}
}

void Character::RotateWeapon(bool direction)
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

void Character::SetWeaponRotation(float rotation)
{
	m_pWeaponSprite->setRotation(rotation);
	m_WeaponAngle = m_pWeaponSprite->getRotation();
}

void Character::Shoot()
{
	if (m_pCurrentKeyFrame->y == 0)
	{
		m_Projectiles.front()->SetRotation(m_WeaponAngle);
	}
	else
	{
		m_Projectiles.front()->SetRotation(360 - m_WeaponAngle);
	}
	m_Projectiles.front()->Shoot();
}

bool Character::IsInAir()
{
	return m_FallingTime > 0.5f;
}

bool Character::IsExplosion()
{
	return m_Projectiles.front()->IsExplosion();
}

bool Character::IsExploded()
{
	return m_Exploded;
}

void Character::OffsetBounds()
{
	m_Projectiles.front()->OffsetBounds();
	Entity::OffsetBounds();
}
