#include "Enemy.h"
#include "Projectile.h"
#include "Config.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Enemy::Enemy(const char* texturePath, const char* weaponTexturePath, std::vector<Explosion*>* explosions, int detectionRadius, int accuracy, int health) : Character(texturePath, weaponTexturePath, explosions, health)
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
	m_pStartPosition->x = Config::GetInstance().GetWindowSizeWidth() * (accuracy / 5);
	m_pStartPosition->y = 0;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.f;
	m_BoundOffsetX = 2;
	m_BoundOffsetY = 1;

	m_pSprite->setTextureRect(sf::IntRect(m_AnimationStart, 0, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	SetPosition(*m_pStartPosition);
	m_pSprite->setScale(SCALE, SCALE);

	m_WalkingSpeed = -20 * SCALE;
	m_SpeedX = -20 * SCALE;
	m_SpeedY = 0;

	m_DetectionRadius = detectionRadius * SCALE;
	m_Accuracy = accuracy * SCALE;

	m_pWeaponSprite->setTextureRect(sf::IntRect(0, 0, m_pWeaponSprite->getGlobalBounds().width, m_pWeaponSprite->getGlobalBounds().height));
	m_pWeaponSprite->setOrigin(sf::Vector2f(m_pWeaponSprite->getGlobalBounds().width / 2, m_pWeaponSprite->getGlobalBounds().height / 2));
	m_pWeaponSprite->setPosition(sf::Vector2f(m_pStartPosition->x + 1 * SCALE, m_pStartPosition->y + 7 * SCALE));
	m_pWeaponSprite->setScale(SCALE, SCALE);
	m_WeaponAngle = 2;
	SetWeaponRotation(m_WeaponAngle);

	m_Jumping = false;

	m_FullHealth = health;

	m_Projectiles.push(new Projectile("..\\resources\\projectile.png", m_pExplosions, (-175 * SCALE + m_DetectionRadius), (100 * SCALE + m_DetectionRadius) / 2));
	m_Projectiles.front()->SetDirection(-1);

	m_RotateDown = true;

	m_Shoot = false;

	m_LastDistance = 0;
	m_SecondLastDistance = 0;
	m_LastAngle = 2;

	UpdateBounds();
}

Enemy::~Enemy()
{
}

void Enemy::CheckForPlayer(int playerX, int playerY)
{
	if (!m_Projectiles.front()->IsShooting() && sqrt(pow(m_pSprite->getPosition().x - playerX, 2) + pow(m_pSprite->getPosition().y - playerY, 2)) <= m_DetectionRadius && m_pSprite->getPosition().x > playerX)
	{
		m_WalkingSpeed = 0;
		AimForPlayer(playerX, playerY);
	}
	else
	{
		m_WalkingSpeed = -20 * SCALE;
	}
}

void Enemy::AimForPlayer(int playerX, int playerY)
{
	if (!IsInAir() && !m_Projectiles.front()->IsShooting() && abs(m_SpeedX) <= 0.001)
	{
		if (m_Shoot)
		{
			Shoot();
			m_LastDistance = 0;
			m_SecondLastDistance = 0;
			m_LastAngle = 2;
		}
		if (m_WeaponAngle < 1.f || m_WeaponAngle > 45.f)
		{
			SetWeaponRotation(m_LastAngle);
			m_RotateDown = !m_RotateDown;
		}
		float g = 9.81 * 20 * SCALE;

		float v_0 = (100 * SCALE + m_DetectionRadius) / 2;
		float y_0 = abs(m_Projectiles.front()->GetPosition().y - playerY);

		float angle = m_WeaponAngle;
		float x = abs(pow(v_0, 2) / (2 * g) * (1 + sqrt(1 + (2 * g * y_0 / (pow(v_0, 2) * pow(sin(angle * 3.141592654f / 180), 2))))) * sin(2 * angle * 3.141592654f / 180));
		
		float lastDiff = abs(m_LastDistance - (m_Projectiles.front()->GetPosition().x - playerX));
		float secondLastDiff = abs(m_SecondLastDistance - (m_Projectiles.front()->GetPosition().x - playerX));
		float diff = abs(x - (m_Projectiles.front()->GetPosition().x - playerX));
		if (lastDiff < diff && m_LastDistance != 0)
		{
			m_RotateDown = !m_RotateDown;
		}
		if ((lastDiff < diff && secondLastDiff > lastDiff && m_SecondLastDistance != 0))
		{
			//printf("SHOOT!\n");
			m_Shoot = true;
		}
		else
		{
			m_pSprite->setTextureRect(sf::IntRect((2 + 2 * (angle > 22)) * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
			if (angle == 45 || angle == 1)
			{
				m_RotateDown = !m_RotateDown;
			}
			RotateWeapon(m_RotateDown);

			//printf("Angle: %f\nTarget distance: %f\nDistance: %f\n", angle, m_Projectiles.front()->GetPosition().x - playerX, x);
			//printf("Last distance: %f\nSecond last distance: %f\n\n", m_LastDistance, m_SecondLastDistance);

			m_SecondLastDistance = m_LastDistance;
			m_LastDistance = x;
		}

		if (abs(m_Projectiles.front()->GetPosition().x - playerX) < (-175 * SCALE + m_DetectionRadius) && abs(m_Projectiles.front()->GetPosition().y - playerY) < (-175 * SCALE + m_DetectionRadius))
		{
			m_pSprite->setTextureRect(sf::IntRect(5 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
			SetWeaponRotation(315);
			m_Shoot = true;
		}
		
		//if ((m_SecondLastDistance > m_LastDistance && m_LastDistance < x && abs(m_LastDistance - (m_Projectiles.front()->GetPosition().x - playerX)) > 50) && abs(m_LastDistance - (m_Projectiles.front()->GetPosition().x - playerX)) < m_LastDistance)
		//{
		//	m_LastAngle = angle;
		//	SetWeaponRotation(315);
		//	m_pSprite->setTextureRect(sf::IntRect(5 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

		//	printf("SHOOT DOWN!\n");
		//	//Shoot();
		//}
		//else if ((m_SecondLastDistance < m_LastDistance && m_LastDistance > x && abs(m_LastDistance - (m_Projectiles.front()->GetPosition().x - playerX)) > 50) && abs(m_LastDistance - (m_Projectiles.front()->GetPosition().x - playerX)) > m_LastDistance)
		//{
		//	m_LastAngle = angle;
		//	SetWeaponRotation(45);
		//	m_pSprite->setTextureRect(sf::IntRect(4 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

		//	printf("SHOOT UP!\n");
		//	//Shoot();
		//}
	}

}

void Enemy::RotateWeapon(bool direction)
{
	if ((m_WeaponAngle >= 0 && m_WeaponAngle < 45) || (m_WeaponAngle <= 360 && m_WeaponAngle > 315) || (m_WeaponAngle == 45 && !direction) || (m_WeaponAngle == 315 && direction))
	{
		if (m_pCurrentKeyFrame->y == 0)
		{
			m_pWeaponSprite->rotate((2 * direction - 1));
			m_WeaponAngle = m_pWeaponSprite->getRotation();
		}
		else
		{
			m_pWeaponSprite->rotate(-(2 * direction - 1));
			m_WeaponAngle = (360 - m_pWeaponSprite->getRotation());
		}
	}
}

void Enemy::Update(float dt, sf::RenderWindow * window, float offset)
{
	if (m_RightBound >= (int)(offset + (Config::GetInstance().GetWindowSizeWidth() / 2)) + m_OffsetBounds + m_pKeyFrameSize->x * 2 * SCALE)
	{
		m_Falling = false;
	}

	Move(dt);

	if (!m_Projectiles.front()->IsShooting())
	{
		m_Projectiles.front()->SetPosition(m_pWeaponSprite->getPosition().x, m_pWeaponSprite->getPosition().y - 5 * SCALE);
	}
	else
	{
		m_Projectiles.front()->Update(dt, window, offset);
	}

	if ((m_SpeedX != 0 || m_SpeedY == 0) && m_WalkingSpeed != 0)
	{
		m_KeyFrameDuration += dt;
		if (m_KeyFrameDuration > m_AnimationSpeed)
		{
			m_pCurrentKeyFrame->x = ((m_pCurrentKeyFrame->x - m_AnimationStart) + 1) % m_AnimationLength + m_AnimationStart;
			m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

			m_KeyFrameDuration = 0;
		}
	}

	if (m_RightBound < (int)(offset - (Config::GetInstance().GetWindowSizeWidth() / 2)) + m_OffsetBounds)
	{
		m_pSprite->move(Config::GetInstance().GetWindowSizeWidth() * 2, -m_TopBound);
		m_pWeaponSprite->move(Config::GetInstance().GetWindowSizeWidth() * 2, -m_TopBound);
		m_SpeedX = 0;
		m_SpeedY = 0;
		UpdateBounds();
		m_CurrentHealth = m_FullHealth;
	}
	/*if (m_RightBound >= (int)(offset + (Config::GetInstance().GetWindowSizeWidth() / 2)) + m_OffsetBounds && m_SpeedX > 0)
	{
		m_SpeedX *= -1;
	}*/
	if (m_TopBound > Config::GetInstance().GetWindowSizeHeight())
	{
		m_CurrentHealth = 0;
	}

	if (m_CurrentHealth == 0)
	{
		printf("DEAD!\n");
		m_pSprite->move(Config::GetInstance().GetWindowSizeWidth() * 2, -m_TopBound);
		m_pWeaponSprite->move(Config::GetInstance().GetWindowSizeWidth() * 2, -m_TopBound);
		m_SpeedX = 0;
		m_SpeedY = 0;
		UpdateBounds();
		SetWeaponRotation(2);
		m_RotateDown = true;
		m_CurrentHealth = m_FullHealth;
	}

	UpdateBounds();
}

void Enemy::Render(sf::RenderWindow * window)
{
	m_Projectiles.front()->Render(window);
	if (m_WalkingSpeed == 0)
	{
		window->draw(*m_pWeaponSprite);
	}

	window->draw(*m_pSprite);
}
