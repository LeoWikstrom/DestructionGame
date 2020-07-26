#include "Player.h"
#include "Config.h"
#include "Projectile.h"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Player::Player(const char* texturePath, const char* weaponTexturePath) : Character(texturePath, weaponTexturePath), m_pCoverSprite(new sf::Sprite), m_pCoverTex(new sf::Texture)
{
	m_pKeyFrameSize->x = 24;
	m_pKeyFrameSize->y = 25;
	m_pSpriteSheetSize->x = 6;
	m_pSpriteSheetSize->y = 2;
	m_BaseKeyFrame = 0;
	m_AnimationLength = 4;
	m_AnimationStart = 0;
	m_pCurrentKeyFrame->x = m_AnimationStart;
	m_pCurrentKeyFrame->y = 0;
	m_pStartPosition->x = Config::GetInstance().GetWindowSizeWidth() / 2;
	m_pStartPosition->y = 200;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.15f;

	m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pSprite->setPosition(*m_pStartPosition);

	m_WalkingSpeed = 0;
	m_SpeedX = 0;
	m_SpeedY = 0;

	m_ShowWeapon = false;
	m_pWeaponSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pWeaponSprite->setOrigin(sf::Vector2f(m_pKeyFrameSize->x / 2, 18));
	m_pWeaponSprite->setPosition(sf::Vector2f(m_pStartPosition->x + m_pKeyFrameSize->x / 2, m_pStartPosition->y + 18));
	m_WeaponAngle = 0;

	m_pCoverTex->loadFromFile("..\\resources\\cape.png");
	m_pCoverSprite->setTexture(*m_pCoverTex);
	m_pCoverSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pCoverSprite->setPosition(*m_pStartPosition);

	m_Jumping = false;

}

Player::~Player()
{
	delete m_pCoverTex;
	delete m_pCoverSprite;
}

void Player::SetWalkingSpeed(int speed)
{
	m_WalkingSpeed = speed;
	if (!IsInAir())
	{
		m_SpeedX = m_WalkingSpeed;
	}
	else
	{
		m_SpeedX = m_WalkingSpeed / 2;
	}
}

void Player::ShowWeapon(bool show)
{
	m_ShowWeapon = show;
	if (m_ShowWeapon)
	{
		m_BaseKeyFrame = 2;
		m_AnimationLength = 2;
		m_AnimationStart = 1;
	}
	else
	{
		m_BaseKeyFrame = 0;
		m_AnimationLength = 4;
		m_AnimationStart = 0;
	}
}

void Player::Jump()
{
	if (!IsInAir())
	{
		m_SpeedY = -100;
		ShowWeapon(false);
		m_WalkingSpeed = 50 * -(2 * m_pCurrentKeyFrame->y - 1) * (m_WalkingSpeed != 0);
		m_Jumping = true;
	}
}

bool Player::IsWeaponOut()
{
	return m_ShowWeapon;
}

void Player::Update(float dt, sf::RenderWindow * window)
{
	if (IsInAir())
	{
		ShowWeapon(false);
	}
	if (m_WalkingSpeed > 0)
	{
		if (m_pCurrentKeyFrame->y == 1)
		{
			m_pWeaponSprite->setRotation(m_WeaponAngle);
			m_Projectiles.front()->SetDirection(1);
		}
		m_pCurrentKeyFrame->y = 0;
	}
	else if (m_WalkingSpeed < 0)
	{
		if (m_pCurrentKeyFrame->y == 0)
		{
			m_pWeaponSprite->setRotation(360 - m_WeaponAngle);
			m_Projectiles.front()->SetDirection(-1);
		}
		m_pCurrentKeyFrame->y = 1;
	}
	Move(dt);
	m_pCoverSprite->setPosition(m_pSprite->getPosition());

	if (!m_Projectiles.front()->IsShooting())
	{
		m_Projectiles.front()->SetPosition(m_pWeaponSprite->getPosition().x, m_pWeaponSprite->getPosition().y - 5);
	}
	else
	{
		m_Projectiles.front()->Update(dt, window);
	}
	if (m_Projectiles.front()->IsExplosion())
	{
		m_Projectiles.front()->UpdateExplosion(dt);
	}

	if (m_WalkingSpeed != 0)
	{
		m_KeyFrameDuration += dt;
		if (m_KeyFrameDuration > m_AnimationSpeed)
		{
			m_pCurrentKeyFrame->x = ((m_pCurrentKeyFrame->x - m_AnimationStart) + 1) % m_AnimationLength + m_AnimationStart;
			m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
			m_pWeaponSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
			m_pCoverSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

			m_KeyFrameDuration = 0;
		}
	}
	else if (m_SpeedY == 0)
	{
		m_pCurrentKeyFrame->x = m_BaseKeyFrame;
		m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
		m_pWeaponSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
		m_pCoverSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

		m_KeyFrameDuration = 0.15f;
	}
	else
	{
		m_pSprite->setTextureRect(sf::IntRect(4 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
		m_pWeaponSprite->setTextureRect(sf::IntRect(0 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
		m_pCoverSprite->setTextureRect(sf::IntRect(0 * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	}

	if (m_pSprite->getPosition().x <= -16)
	{
		m_pSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pSprite->getPosition().y);
	}
	else if (m_pSprite->getPosition().x >= Config::GetInstance().GetWindowSizeWidth())
	{
		m_pSprite->setPosition(-16, m_pSprite->getPosition().y);
	}

	if (m_pWeaponSprite->getPosition().x <= -16)
	{
		m_pWeaponSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pWeaponSprite->getPosition().y);
	}
	else if (m_pWeaponSprite->getPosition().x >= Config::GetInstance().GetWindowSizeWidth())
	{
		m_pWeaponSprite->setPosition(-16, m_pWeaponSprite->getPosition().y);
	}

	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 7;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pSprite->getGlobalBounds().width - 7;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 1;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pSprite->getGlobalBounds().height - 1;
}

void Player::Render(sf::RenderWindow * window)
{
	m_Projectiles.front()->Render(window);
	window->draw(*m_pSprite);
	if (m_ShowWeapon)
	{
		window->draw(*m_pWeaponSprite);
		window->draw(*m_pCoverSprite);
	}
	
}
