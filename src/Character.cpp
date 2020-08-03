#include "Character.h"
#include "Config.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <algorithm>
#include "Explosion.h"

Character::Character(const char * texturePath, const char * weaponTexturePath, std::vector<Explosion*>* explosions, unsigned int health) : Entity(texturePath), m_pWeaponTex(new sf::Texture), m_pWeaponSprite(new sf::Sprite), m_MaxHealth(health), m_CurrentHealth(health), m_Invulnerable(false), m_pExplosions(explosions)
{
	m_pWeaponTex->loadFromFile(weaponTexturePath);
	m_pWeaponSprite->setTexture(*m_pWeaponTex);

	m_Falling = false;
	m_FallingTime = 0;

	m_Weight = 2;

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
}

bool Character::CheckTerrainCollision(sf::Image * terrain)
{
	m_Falling = true;
	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight() && m_TopBound > 0)
	{
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, m_BottomBound) != SKY_COLOUR)
			{
				bool walkableRight = true, walkableLeft = true, fullStop = false;
				int height = 0;

				for (int j = m_BottomBound; j >= m_BottomBound - 5 * SCALE; --j)
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

				for (int j = m_BottomBound - 5 * SCALE - 1; j >= m_TopBound; --j)
				{
					if (m_SpeedX < 0)
					{
						if (terrain->getPixel(m_LeftBound, j) != SKY_COLOUR)
						{
							walkableLeft = false;
							fullStop = true;
						}
					}
					else if (m_SpeedX > 0)
					{
						if (terrain->getPixel(m_RightBound, j) != SKY_COLOUR)
						{
							walkableRight = false;
							fullStop = true;
						}
					}
				}

				if (m_SpeedY >= 0)
				{
					if (!m_Jumping)
					{
						m_SpeedY = 0;
					}
					m_Falling = false;
					m_FallingTime = 0;
					m_Exploded = false;
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

			if (terrain->getPixel(i, m_TopBound) != SKY_COLOUR)
			{
				if (m_SpeedY < 0)
				{
					m_SpeedY *= -0.1;
				}

				break;
			}
		}

		for (int i = m_TopBound; i <= m_BottomBound; ++i)
		{
			bool fullStop = false;
			if (m_SpeedX < 0)
			{
				if (terrain->getPixel(m_LeftBound, i) != SKY_COLOUR)
				{
					fullStop = true;
				}
			}
			else if (m_SpeedX > 0)
			{
				if (terrain->getPixel(m_RightBound, i) != SKY_COLOUR)
				{
					fullStop = true;
				}
			}

			if (IsInAir() && fullStop)
			{
				m_SpeedX *= -0.25;
			}
		}
	}

	if (!m_Exploded && m_Invulnerable)
	{
		m_Invulnerable = false;
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

bool Character::IsExploded()
{
	return m_Exploded;
}

void Character::OffsetBounds()
{
	m_Projectiles.front()->OffsetBounds();
	Entity::OffsetBounds();
}

unsigned int Character::GetCurrentHealth()
{
	return m_CurrentHealth;
}

unsigned int Character::GetMaxHealth()
{
	return m_MaxHealth;
}

void Character::Explode(sf::Vector2f angle)
{
	if (!m_Exploded)
	{
		m_SpeedY = (angle.y / m_Weight) * SCALE;
		m_SpeedX = (angle.x / m_Weight) * SCALE;
		m_Exploded = true;
		m_Invulnerable = true;
		m_CurrentHealth = m_CurrentHealth > 0 ? m_CurrentHealth - 1 : 0;
	}
}
