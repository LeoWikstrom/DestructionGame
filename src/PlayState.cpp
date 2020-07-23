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

	m_pPlayer = new Player("..\\resources\\player.png");

	m_pTerrain->GenerateTerrain(300, 300, Config::GetInstance().GetWindowSizeHeight(), 0, Config::GetInstance().GetWindowSizeWidth(), 50, 4);

	m_WasSpacePressed = false;

	m_Enemies.push(new Enemy("..\\resources\\small_enemy.png"));

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

		if (isWPressed && !m_WasWPressed)
		{
			m_WasWPressed = true;
		}
		else if (!isWPressed && m_WasWPressed)
		{
			m_WasWPressed = false;
		}
		if (isAPressed && !m_WasAPressed)
		{
			m_pPlayer->SetWalkingSpeed(-30);
			m_WasAPressed = true;
		}
		else if (!isAPressed && m_WasAPressed)
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
		if (isDPressed && !m_WasDPressed)
		{
			m_pPlayer->SetWalkingSpeed(30);
			m_WasDPressed = true;
		}
		else if (!isDPressed && m_WasDPressed)
		{
			m_pPlayer->SetWalkingSpeed(0);
			m_WasDPressed = false;
			m_WasAPressed = false;
		}

		if (isSpacePressed && !m_WasSpacePressed)
		{
			m_pTerrain->DrawCircle(rand() % Config::GetInstance().GetWindowSizeWidth(), rand() % 350 + (Config::GetInstance().GetWindowSizeHeight() - 350), rand() % 100);
			m_WasSpacePressed = true;
		}
		else if (!isSpacePressed && m_WasSpacePressed)
		{
			m_WasSpacePressed = false;
		}
	}

	m_pPlayer->CheckTerrainCollision(&m_pTerrain->GetTerrain());
	m_pPlayer->Update(dt, window);
	
	m_Enemies.front()->CheckTerrainCollision(&m_pTerrain->GetTerrain());
	m_Enemies.front()->Update(dt, window);
}

void PlayState::Render(sf::RenderWindow * window)
{
	window->clear();
	m_pTerrain->Render(window);
	m_pPlayer->Render(window);
	m_Enemies.front()->Render(window);
	window->display();
}
