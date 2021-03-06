#include "PlayState.h"
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Player.h"
#include "Terrain.h"
#include "Config.h"
#include "Enemy.h"
#include <iostream>
#include "Score.h"
#include "GameOverState.h"
#include "Explosion.h"

PlayState::PlayState(Game * game, sf::Font* font) : GameState(game), m_pTerrain(new Terrain()), m_CameraOffset(0.f), m_nextTerrainUpdate(Config::GetInstance().GetWindowSizeWidth() * 3 / 2), m_pLifeTex(new sf::Texture()), m_pVisScore(new sf::Text()), m_pFont(font)
{

	m_pPlayer = new Player("..\\resources\\player.png", "..\\resources\\gun_player.png", &m_Explosions);

	m_pTerrain->InitTerrain(200 * SCALE, 200 * SCALE, Config::GetInstance().GetWindowSizeHeight(), 0, Config::GetInstance().GetWindowSizeWidth(), 50 * SCALE, 4);
	m_pTerrain->GenSecondTerrain(rand() % (Config::GetInstance().GetWindowSizeHeight() - 100 * SCALE) + 50 * SCALE, Config::GetInstance().GetWindowSizeHeight(), 0, 50 * SCALE, 4);
	std::cout << "Generated second terrain, offset is: " << m_CameraOffset << std::endl;

	m_WasSpacePressed = false;
	m_WasWPressed = false;
	m_WasAPressed = false;
	m_WasSPressed = false;
	m_WasDPressed = false;
	m_WasUpPressed = false;
	m_WasLeftPressed = false;
	m_WasDownPressed = false;
	m_WasRightPressed = false;
	m_WasTerrainGenerated = false;

	for (int i = 1; i <= 6; ++i)
	{
		char path[] = "..\\resources\\blobert1.png";
		path[20] = ((i + (i % 2)) / 2) + 48;
		m_Enemies.push_back(new Enemy(path, "..\\resources\\gun_small.png", &m_Explosions, 200 + 25 * (((i + (i % 2)) / 2) - 1), i, (i + (i % 2)) / 2));
	}

	m_pLifeTex->loadFromFile("..\\resources\\heart.png");
	m_ppLifeSprites = new sf::Sprite * [m_pPlayer->GetMaxHealth()];
	for (unsigned int i = 0; i < m_pPlayer->GetMaxHealth(); i++)
	{
		m_ppLifeSprites[i] = new sf::Sprite();
		m_ppLifeSprites[i]->setTexture(*m_pLifeTex);
		m_ppLifeSprites[i]->setPosition(8 * i * SCALE, 0);
		m_ppLifeSprites[i]->setScale(2*SCALE, 2*SCALE);
	}

	m_pVisScore->setFont(*m_pFont);
	m_pVisScore->setScale(SCALE, SCALE);
}

PlayState::~PlayState()
{
	delete m_pLifeTex;

	for (int i = 0; i < m_pPlayer->GetMaxHealth(); i++)
	{
		delete m_ppLifeSprites[i];
	}
	delete[] m_ppLifeSprites;
	delete m_pPlayer;
	delete m_pTerrain;

	for (int i = 0; i < m_Enemies.size(); i++)
	{
		delete m_Enemies[i];
	}
	m_Enemies.clear();

	for (int i = 0; i < m_Explosions.size(); i++)
	{
		delete m_Explosions[i];
	}
	m_Explosions.clear();


	delete m_pVisScore;
}

void PlayState::Update(float dt, sf::RenderWindow * window)
{
	if (m_pPlayer->GetCurrentHealth() == 0 && !m_pPlayer->IsExploded())
	{
		if (m_pLastState == nullptr)
			window->close();
		PopState();
		return;
	}
	//Input
	sf::Event event;
	if (window->hasFocus())
	{
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window->close();
			}
		}

		bool isSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
		bool isWPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
		bool isAPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
		bool isSPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
		bool isDPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
		bool isUpPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
		bool isLeftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
		bool isDownPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
		bool isRightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);

		if (isWPressed && !m_WasWPressed)
		{
			m_pPlayer->Jump();
			m_WasWPressed = true;
		}
		else if (!isWPressed && m_WasWPressed)
		{
			m_WasWPressed = false;
		}
		if (isAPressed && !m_WasAPressed && !m_pPlayer->IsExploded())
		{
			m_pPlayer->SetWalkingSpeed(-50 * SCALE + 20 * SCALE * m_pPlayer->IsWeaponOut());
			m_WasAPressed = true;
		}
		else if (!isAPressed && m_WasAPressed && !m_pPlayer->IsExploded())
		{
			m_pPlayer->SetWalkingSpeed(0);
			m_WasAPressed = false;
			m_WasDPressed = false;
		}
		if (isSPressed && !m_WasSPressed)
		{
			m_WasSPressed = true;
		}
		else if (!isSPressed && m_WasSPressed)
		{
			m_WasSPressed = false;
		}
		if (isDPressed && !m_WasDPressed && !m_pPlayer->IsExploded())
		{
			m_pPlayer->SetWalkingSpeed(50 * SCALE - 20 * SCALE * m_pPlayer->IsWeaponOut());
			m_WasDPressed = true;
		}
		else if (!isDPressed && m_WasDPressed && !m_pPlayer->IsExploded())
		{
			m_pPlayer->SetWalkingSpeed(0);
			m_WasDPressed = false;
			m_WasAPressed = false;
		}

		if (isSpacePressed && !m_WasSpacePressed && m_pPlayer->IsWeaponOut())
		{
			m_pPlayer->Shoot();
			m_WasSpacePressed = true;
		}
		else if (!isSpacePressed && m_WasSpacePressed)
		{
			m_WasSpacePressed = false;
		}

		if (isRightPressed && !m_WasRightPressed)
		{
			m_pPlayer->ShowWeapon(true);
			m_WasRightPressed = true;

			if (isDPressed && !m_pPlayer->IsExploded())
			{
				m_pPlayer->SetWalkingSpeed(50 * SCALE - 20 * SCALE * m_pPlayer->IsWeaponOut());
			}
			else if (isAPressed && !m_pPlayer->IsExploded())
			{
				m_pPlayer->SetWalkingSpeed(-50 * SCALE + 20 * SCALE * m_pPlayer->IsWeaponOut());
			}
		}
		else if (!isRightPressed && m_WasRightPressed)
		{
			m_WasRightPressed = false;
		}
		if (isLeftPressed && !m_WasLeftPressed)
		{
			m_pPlayer->ShowWeapon(false);
			m_WasLeftPressed = true;

			if (isDPressed && !m_pPlayer->IsExploded())
			{
				m_pPlayer->SetWalkingSpeed(50 * SCALE - 20 * SCALE * m_pPlayer->IsWeaponOut());
			}
			else if (isAPressed && !m_pPlayer->IsExploded())
			{
				m_pPlayer->SetWalkingSpeed(-50 * SCALE + 20 * SCALE * m_pPlayer->IsWeaponOut());
			}
		}
		else if (!isLeftPressed && m_WasLeftPressed)
		{
			m_WasLeftPressed = false;
		}

		if (isUpPressed && !m_WasUpPressed)
		{
			if (m_pPlayer->IsWeaponOut())
			{
				m_pPlayer->RotateWeapon(false);
			}
			m_WasUpPressed = true;
		}
		else if (!isUpPressed && m_WasUpPressed)
		{
			m_WasUpPressed = false;
			m_WasDownPressed = false;
		}
		if (isDownPressed && !m_WasDownPressed)
		{
			if (m_pPlayer->IsWeaponOut())
			{
				m_pPlayer->RotateWeapon(true);
			}
			m_WasDownPressed = true;
		}
		else if (!isDownPressed && m_WasDownPressed)
		{
			m_WasDownPressed = false;
			m_WasUpPressed = false;
		}
	}

	if (m_WasTerrainGenerated)
	{
		m_WasTerrainGenerated = false;
		dt = 0;
	}

	if (m_CameraOffset > m_nextTerrainUpdate)
	{
		m_nextTerrainUpdate += Config::GetInstance().GetWindowSizeWidth();
		m_pTerrain->GenSecondTerrain(rand() % (Config::GetInstance().GetWindowSizeHeight() - 100) + 50, Config::GetInstance().GetWindowSizeHeight(), 0, 50, 4);
		m_pPlayer->OffsetBounds();
		for (int i = 0; i < m_Enemies.size(); ++i)
		{
			m_Enemies[i]->OffsetBounds();
		}
		std::cout << "Generated second terrain, offset is: " << m_CameraOffset << std::endl;
		m_WasTerrainGenerated = true;
	}

	bool isEnemyCollision = false;
	for (int i = 0; i < m_Enemies.size(); ++i)
	{
		isEnemyCollision = m_Enemies[i]->CheckTerrainCollision(&m_pTerrain->GetTerrain());
		if (isEnemyCollision)
		{
			break;
		}
	}

	if (m_pPlayer->CheckTerrainCollision(&m_pTerrain->GetTerrain()) || isEnemyCollision)
	{
		m_pTerrain->Update();
	}

	for (int i = 0; i < m_Explosions.size(); i++)
	{
		m_Explosions[i]->CheckCollision(m_pPlayer);
		for (int j = 0; j < m_Enemies.size(); ++j)
		{
			m_Explosions[i]->CheckCollision(m_Enemies[j]);
		}
	}

	std::vector<unsigned int> expToRemove;
	for (unsigned int i = 0; i < m_Explosions.size(); i++)
	{
		m_Explosions[i]->Update(dt, window);
		if (!m_Explosions[i]->ShouldLive())
			expToRemove.push_back(i);
	}

	m_pPlayer->Update(dt, window, m_CameraOffset);

	std::vector<unsigned int> enemiesToDelete;

	for (unsigned int i = 0; i < m_Enemies.size(); i++)
	{
		if (m_Enemies[i]->GetCurrentHealth() == 0)
		{
			Score::AddKillScore(5 * m_Enemies[i]->GetMaxHealth() * m_Enemies[i]->GetMaxHealth());
			enemiesToDelete.push_back(i);
		}
		else if (m_Enemies[i]->GetPosition().x < (m_CameraOffset - Config::GetInstance().GetWindowSizeWidth()))
		{
			enemiesToDelete.push_back(i);
		}
	}

	std::rotate(m_Enemies.begin(), m_Enemies.begin() + enemiesToDelete.size(), m_Enemies.end());

	for (int i = 0; i < m_Enemies.size(); ++i)
	{
		m_Enemies[i]->Update(dt, window, m_CameraOffset);
		m_Enemies[i]->CheckForPlayer(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);
	}

	for (int i = expToRemove.size() - 1; i >= 0; i--)
	{
		delete m_Explosions[expToRemove[i]];
		m_Explosions.erase(m_Explosions.begin() + expToRemove[i]);
	}

	// CameraControl
	m_CameraOffset = m_pPlayer->GetPosition().x > m_CameraOffset ? m_pPlayer->GetPosition().x : m_CameraOffset;
	
	sf::View view = window->getView();
	view.setCenter(m_CameraOffset, view.getCenter().y);
	window->setView(view);


	for (unsigned int i = 0; i < m_pPlayer->GetMaxHealth(); i++)
	{
		m_ppLifeSprites[i]->setPosition(m_CameraOffset - Config::GetInstance().GetWindowSizeWidth() / 2 + 20 * i * SCALE + 10 * SCALE, 0);
	}

	if (m_pPlayer->GetCurrentHealth() > 0)
	{
		Score::SetDistance((m_CameraOffset - Config::GetInstance().GetWindowSizeWidth() / 2) / 10);
	}

	m_pVisScore->setString(std::to_string(Score::GetScore()));
	m_pVisScore->setPosition(m_CameraOffset, 0);

	if (m_pPlayer->GetCurrentHealth() == 0 && !m_pPlayer->IsExploded())
	{
		// Reset the view...
		window->setView(sf::View(sf::Vector2f(Config::GetInstance().GetWindowSizeWidth() / 2, Config::GetInstance().GetWindowSizeHeight() / 2), sf::Vector2f(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight())));
		
		ChangeState(new GameOverState(m_pGame, m_pFont));
	}
}

void PlayState::Render(sf::RenderWindow * window)
{
	window->clear();
	m_pTerrain->Render(window);
	for (int i = 0; i < m_Enemies.size(); ++i)
	{
		m_Enemies[i]->Render(window);
	}
	m_pPlayer->Render(window);

	for (int i = 0; i < m_Explosions.size(); i++)
	{
		m_Explosions[i]->Render(window);
	}

	for (unsigned int i = 0; i < m_pPlayer->GetCurrentHealth(); i++)
	{
		window->draw(*m_ppLifeSprites[i]);
	}

	window->draw(*m_pVisScore);
	window->display();
}
