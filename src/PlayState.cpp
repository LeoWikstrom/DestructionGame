#include "PlayState.h"
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Player.h"
#include "Terrain.h"
#include "Config.h"
#include "Enemy.h"

PlayState::PlayState(Game * game) : GameState(game), m_pTerrain(new Terrain())
{
	m_pFont = new sf::Font();
	char* winDir = getenv("WinDir"); //Get the window directory
	m_pFont->loadFromFile(std::string(winDir) + "\\Fonts\\Ebrima.ttf");

	m_pPlayer = new Player("..\\resources\\player.png", "..\\resources\\gun_player.png");

	m_pTerrain->GenerateTerrain(200 * SCALE, 200 * SCALE, Config::GetInstance().GetWindowSizeHeight(), 0, Config::GetInstance().GetWindowSizeWidth(), 50 * SCALE, 4);

	m_WasSpacePressed = false;
	m_WasWPressed = false;
	m_WasAPressed = false;
	m_WasSPressed = false;
	m_WasDPressed = false;
	m_WasUpPressed = false;
	m_WasLeftPressed = false;
	m_WasDownPressed = false;
	m_WasRightPressed = false;

	m_Enemies.push(new Enemy("..\\resources\\small_enemy.png", "..\\resources\\gun_small.png", 200, 5));

	sf::Image terrain = m_pTerrain->GetTerrain();
}

PlayState::~PlayState()
{
	delete m_pFont;
	delete m_pPlayer;
	delete m_pTerrain;
	while (!m_Enemies.empty())
	{
		delete m_Enemies.front();
		m_Enemies.pop();
	}
}

void PlayState::Update(float dt, sf::RenderWindow * window)
{
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

	if (m_pPlayer->CheckTerrainCollision(&m_pTerrain->GetTerrain()) || m_pPlayer->IsExplosion())
	{
		m_pTerrain->Update();
	}
	m_pPlayer->Update(dt, window);
	
	m_Enemies.front()->CheckForPlayer(m_pPlayer->GetPosition().x, m_pPlayer->GetPosition().y);
	m_Enemies.front()->CheckTerrainCollision(&m_pTerrain->GetTerrain());
	m_Enemies.front()->Update(dt, window);
}

void PlayState::Render(sf::RenderWindow * window)
{
	window->clear();
	m_pTerrain->Render(window);
	m_Enemies.front()->Render(window);
	m_pPlayer->Render(window);
	window->display();
}
