#include "Projectile.h"
#include "Config.h"
#include <SFML/Graphics.hpp>

Projectile::Projectile(const char * texturePath, int damage, int power) : Entity(texturePath), m_pTerrainImage(nullptr)
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
	m_pStartPosition->x = 100 * SCALE;
	m_pStartPosition->y = 200 * SCALE;
	m_AnimationSpeed = 0.15f;
	m_KeyFrameDuration = 0.15f;

	m_pSprite->setTextureRect(sf::IntRect(m_pCurrentKeyFrame->x * m_pKeyFrameSize->x, m_pCurrentKeyFrame->y * m_pKeyFrameSize->y, m_pKeyFrameSize->x, m_pKeyFrameSize->y));
	m_pSprite->setPosition(*m_pStartPosition);
	m_pSprite->setOrigin(m_pKeyFrameSize->x / 2, 0);
}

Projectile::~Projectile()
{
	while (!m_Explosions.empty())
	{
		delete m_Explosions.back();
		m_Explosions.pop_back();
	}
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

		m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 2 * SCALE;
		m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pKeyFrameSize->x * SCALE - 2 * SCALE;
		m_TopBound = (int)m_pSprite->getGlobalBounds().top + 2 * SCALE;
		m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pKeyFrameSize->y * SCALE - 2 * SCALE;
	}
}

bool Projectile::IsShooting()
{
	return m_Shooting;
}

void Projectile::UpdateExplosion(float dt)
{
	if (!m_Explosions.empty())
	{
		sf::RenderTexture renderTex;
		sf::Texture texture;
		sf::Sprite sprite;
		texture.loadFromImage(*m_pTerrainImage);
		sprite.setTexture(texture);
		renderTex.create(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight());
		renderTex.clear();
		renderTex.draw(sprite);

		for (int i = 0; i < m_Explosions.size(); ++i)
		{
			int height = Config::GetInstance().GetWindowSizeHeight(),
				width = Config::GetInstance().GetWindowSizeWidth();
			sf::Color colourCenter;
			if (m_Explosions[i]->y >= 0 && m_Explosions[i]->y < height)
			{
				colourCenter = m_pTerrainImage->getPixel(m_Explosions[i]->x, m_Explosions[i]->y);
			}
			sf::CircleShape explosion(m_Damage);
			explosion.setFillColor(sf::Color(0, 255, 255, 2500*dt));
			explosion.setOrigin(m_Damage, m_Damage);
			if (colourCenter.r == 255)
			{
				explosion.setRadius(5);
				explosion.setFillColor(sf::Color(254, 0, 0));
				explosion.setOrigin(5, 5);
			}

			explosion.setPointCount(40);
			explosion.setPosition(*m_Explosions[i]);
			renderTex.draw(explosion);
			int pointCount = explosion.getPointCount();
			float	circleUp = m_Explosions[i]->y - (m_Damage - 1),
					circleRight = m_Explosions[i]->x + (m_Damage - 1),
					circleDown = m_Explosions[i]->y + (m_Damage - 1),
					circleLeft = m_Explosions[i]->x - (m_Damage - 1);
			sf::Color colourUp = sf::Color(0, 0, 0, 0);
			if (circleUp >= 0 && circleUp < height && m_Explosions[i]->x >= 0 && m_Explosions[i]->x < width)
			{
				colourUp = m_pTerrainImage->getPixel(m_Explosions[i]->x, circleUp);
			}
			sf::Color colourRight;
			if (circleRight >= 0 && circleRight < width && m_Explosions[i]->y >= 0 && m_Explosions[i]->y < height)
			{
				colourRight = m_pTerrainImage->getPixel(circleRight, m_Explosions[i]->y);
			}
			sf::Color colourDown;
			if (circleDown >= 0 && circleDown < height && m_Explosions[i]->x >= 0 && m_Explosions[i]->x < width)
			{
				colourDown = m_pTerrainImage->getPixel(m_Explosions[i]->x, circleDown);
			}
			sf::Color colourLeft;
			if (circleLeft >= 0 && circleLeft < width && m_Explosions[i]->y >= 0 && m_Explosions[i]->y < height)
			{
				colourLeft = m_pTerrainImage->getPixel(circleLeft, m_Explosions[i]->y);
			}

			if ((colourUp.g >= 250 || colourUp.a == 0) && (colourRight.g >= 250 || colourRight.a == 0) && (colourDown.g >= 250 || colourDown.a == 0) && (colourLeft.g >= 250 ||colourLeft.a == 0) && (colourCenter.g >= 250 || colourCenter.a == 0))
			{
				explosion.setFillColor(SKY_COLOUR);
			}
			renderTex.draw(explosion);

			if ((colourUp == SKY_COLOUR || colourUp.a == 0) && (colourRight == SKY_COLOUR || colourRight.a == 0) && (colourDown == SKY_COLOUR || colourDown.a == 0) && (colourLeft == SKY_COLOUR || colourLeft.a == 0) && (colourCenter == SKY_COLOUR || colourCenter.a == 0) && explosion.getFillColor() == SKY_COLOUR)
			{
				delete m_Explosions[i];
				m_Explosions.erase(m_Explosions.begin() + i);
				i = -1;
			}

			//printf("colourUp %d\ncolourRight %d\ncolourDown %d\ncolourLeft %d\ncolourCenter %d\ncolourExplosion %d\n\n", colourUp.r, colourRight.r, colourDown.r, colourLeft.r, colourCenter.r, explosion.getFillColor().r);
		}
		renderTex.display();

		*m_pTerrainImage = renderTex.getTexture().copyToImage();

	}
}

bool Projectile::IsExplosion()
{
	return !m_Explosions.empty();
}

void Projectile::Move(float dt)
{
	m_SpeedY += 9.81 * 20 * SCALE * dt;
	Entity::Move(dt);
}

bool Projectile::CheckTerrainCollision(sf::Image * terrain)
{
	m_pTerrainImage = terrain;

	if (m_BottomBound < Config::GetInstance().GetWindowSizeHeight())
	{
		for (int i = m_LeftBound; i <= m_RightBound; ++i)
		{
			if (terrain->getPixel(i, m_BottomBound) == GROUND_COLOUR || terrain->getPixel(i, m_TopBound) == GROUND_COLOUR)
			{
				m_Shooting = false;
				m_Explosions.push_back(new sf::Vector2f(m_pSprite->getPosition()));

				sf::CircleShape explosions;
				sf::RenderTexture renderTex;
				sf::Texture texture;
				sf::Sprite sprite;

				texture.loadFromImage(*terrain);
				sprite.setTexture(texture);
				renderTex.create(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight());
				renderTex.clear();
				renderTex.draw(sprite);
				int radDecrease = m_Damage / 10;
				for (int i = 0; i < 10; ++i)
				{
					explosions = sf::CircleShape();
					explosions.setRadius(m_Damage - radDecrease * i);
					explosions.setFillColor(sf::Color(165 + 10 * i, 0, 0));
					explosions.setOrigin(m_Damage - radDecrease * i, m_Damage - radDecrease * i);
					explosions.setPosition(m_pSprite->getPosition());
					explosions.setPointCount(40);
					renderTex.draw(explosions);
				}

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
				m_Explosions.push_back(new sf::Vector2f(m_pSprite->getPosition()));

				sf::CircleShape explosions;
				sf::RenderTexture renderTex;
				sf::Texture texture;
				sf::Sprite sprite;

				texture.loadFromImage(*terrain);
				sprite.setTexture(texture);
				renderTex.create(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight());
				renderTex.clear();
				renderTex.draw(sprite);
				int radDecrease = m_Damage / 10;
				for (int i = 0; i < 10; ++i)
				{
					explosions.setRadius(m_Damage - radDecrease * i);
					explosions.setFillColor(sf::Color(165 + 10 * i, 0, 0));
					explosions.setOrigin(m_Damage - radDecrease * i, m_Damage - radDecrease * i);
					explosions.setPosition(m_pSprite->getPosition());
					explosions.setPointCount(40);
					renderTex.draw(explosions);
				}

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
	}
	else
	{
		m_pCurrentKeyFrame->y = 1;
		m_pSprite->setRotation(180 + angle);
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

	if (m_pSprite->getPosition().y > Config::GetInstance().GetWindowSizeHeight())
	{
		m_Shooting = false;
	}

	m_LeftBound = (int)m_pSprite->getGlobalBounds().left + 2 * SCALE;
	m_RightBound = (int)m_pSprite->getGlobalBounds().left + (int)m_pKeyFrameSize->x * SCALE - 2 * SCALE;
	m_TopBound = (int)m_pSprite->getGlobalBounds().top + 2 * SCALE;
	m_BottomBound = (int)m_pSprite->getGlobalBounds().top + (int)m_pKeyFrameSize->y * SCALE - 2 * SCALE;
}

void Projectile::Render(sf::RenderWindow * window)
{
	if (m_Shooting)
	{
		window->draw(*m_pSprite);
	}
}
