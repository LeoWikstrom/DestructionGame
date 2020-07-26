#include "Character.h"
#include "Config.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>

Character::Character(const char * texturePath, const char * weaponTexturePath) : Entity(texturePath), m_pWeaponTex(new sf::Texture), m_pWeaponSprite(new sf::Sprite)
{
	m_pWeaponTex->loadFromFile(weaponTexturePath);
	m_pWeaponSprite->setTexture(*m_pWeaponTex);

	m_Falling = false;
	m_FallingTime = 0;

	m_Projectiles.push(new Projectile("..\\resources\\projectile.png", 50, 300));
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
		m_SpeedY += 9.81 * 20 * dt;
		m_Jumping = false;
		m_FallingTime += dt;
	}
	m_pSprite->move(m_SpeedX * dt, m_SpeedY * dt);
	m_pWeaponSprite->move(m_SpeedX * dt, m_SpeedY * dt);
}

bool Character::CheckTerrainCollision(sf::Image * terrain)
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
		
			int redDown = terrain->getPixel(i, m_BottomBound).r;
			int redUp = terrain->getPixel(i, m_TopBound).r;
			if (redDown >= 100)
			{
				m_SpeedY = -200;
				m_WalkingSpeed = 50 * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
			}
			if (redUp >= 100 && redUp > redDown)
			{
				m_SpeedY = 200;
				m_WalkingSpeed = 50 * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
			}
		}

		for (int i = m_TopBound; i <= m_BottomBound; ++i)
		{

			int redRight = terrain->getPixel(m_RightBound, i).r;
			int redLeft = terrain->getPixel(m_LeftBound, i).r;
			if (redRight >= 100)
			{
				m_SpeedX = -100;
				m_WalkingSpeed = 50 * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
			}
			if (redLeft >= 100 && redLeft > redRight)
			{
				m_SpeedX = 100;
				m_WalkingSpeed = 50 * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
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
