#include "Projectile.h"
#include "Config.h"
#include <SFML/Graphics.hpp>

Projectile::Projectile(const char * texturePath, int damage, int power) : Entity(texturePath)
{
	m_Damage = damage;
	m_Power = power;
	m_Shooting = false;
	m_SpeedX = 0;
	m_SpeedY = 0;
	m_Direction = 1;

	m_pKeyFrameSize->x = 14;
	m_pKeyFrameSize->y = 9;
	m_pSpriteSheetSize->x = 1;
	m_pSpriteSheetSize->y = 2;
	m_BaseKeyFrame = 0;
	m_AnimationLength = 0;
	m_AnimationStart = 0;
	m_pCurrentKeyFrame->x = m_AnimationStart;
	m_pCurrentKeyFrame->y = 0;
	m_pStartPosition->x = 100;
	m_pStartPosition->y = 200;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.15f;

	m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pSprite->setPosition(*m_pStartPosition);
}

Projectile::~Projectile()
{
}

void Projectile::SetRotation(float angle)
{
	m_pSprite->setRotation(angle);
}

void Projectile::SetDirection(int direction)
{
	m_Direction = direction;
}

void Projectile::Shoot()
{
	if (!m_Shooting)
	{
		m_Shooting = true;

		m_SpeedX = m_Direction * m_Power * cos(m_pSprite->getRotation() * 3.141592654f / 180);
		m_SpeedY = m_Direction * m_Power * sin(m_pSprite->getRotation() * 3.141592654f / 180);

		m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 2;
		m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pKeyFrameSize->x - 2;
		m_TopBound = (int)m_pSprite->getGlobalBounds().top + 2;
		m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pKeyFrameSize->y - 2;
	}
}

bool Projectile::IsShooting()
{
	return m_Shooting;
}

void Projectile::Move(float dt)
{
	m_SpeedY += 9.81 * 20 * dt;
	Entity::Move(dt);
}

bool Projectile::CheckTerrainCollision(sf::Image * terrain)
{
	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight())
	{
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, m_BottomBound) == GROUND_COLOUR || terrain->getPixel(i, m_TopBound) == GROUND_COLOUR)
			{
				m_Shooting = false;

				sf::CircleShape explosion(m_Damage);
				sf::RenderTexture renderTex;
				sf::Texture texture;
				sf::Sprite sprite;

				texture.loadFromImage(*terrain);
				sprite.setTexture(texture);
				renderTex.create(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight());
				renderTex.clear();
				renderTex.draw(sprite);
				explosion.setFillColor(SKY_COLOUR);
				explosion.setOrigin(m_Damage, m_Damage);
				explosion.setPosition(m_pSprite->getPosition());
				renderTex.draw(explosion);

				renderTex.display();

				*terrain = renderTex.getTexture().copyToImage();
				return true;
			}
		}

		for (int j = m_BottomBound; j >= m_TopBound; --j)
		{
			if (terrain->getPixel(m_LeftBound, j) == GROUND_COLOUR || terrain->getPixel(m_RightBound, j) == GROUND_COLOUR)
			{
				m_Shooting = false;

				sf::CircleShape explosion(m_Damage);
				sf::RenderTexture renderTex;
				sf::Texture texture;
				sf::Sprite sprite;

				texture.loadFromImage(*terrain);
				sprite.setTexture(texture);
				renderTex.create(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight());
				renderTex.clear();
				renderTex.draw(sprite);
				explosion.setFillColor(SKY_COLOUR);
				explosion.setOrigin(m_Damage, m_Damage);
				explosion.setPosition(m_pSprite->getPosition());
				renderTex.draw(explosion);

				renderTex.display();

				*terrain = renderTex.getTexture().copyToImage();
				return true;
			}
		}
	}

	return false;
}

void Projectile::Update(float dt, sf::RenderWindow * window)
{
	sf::Vector2f pos1 = m_pSprite->getPosition();
	Move(dt);
	sf::Vector2f pos2 = m_pSprite->getPosition();

	float angle = atan2(pos2.y - pos1.y, pos2.x - pos1.x) * 180 / 3.141592654f;

	if (m_Direction == 1)
	{
		m_pCurrentKeyFrame->y = 0;
		m_pSprite->setRotation(angle);
		m_pSprite->setOrigin(0, 0);
	}
	else
	{
		m_pCurrentKeyFrame->y = 1;
		m_pSprite->setRotation(180 + angle);
		m_pSprite->setOrigin(14, 0);
	}
	m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));

	if (m_pSprite->getPosition().x <= 0)
	{
		m_pSprite->setPosition(Config::GetInstance().GetWindowSizeWidth(), m_pSprite->getPosition().y);
	}
	else if (m_pSprite->getPosition().x >= Config::GetInstance().GetWindowSizeWidth())
	{
		m_pSprite->setPosition(0, m_pSprite->getPosition().y);
	}

	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 2;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pKeyFrameSize->x - 2;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 2;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pKeyFrameSize->y - 2;
}

void Projectile::Render(sf::RenderWindow * window)
{
	if (m_Shooting)
	{
		window->draw(*m_pSprite);
	}
}
