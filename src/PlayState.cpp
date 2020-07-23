#include "PlayState.h"
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Player.h"
#include "Terrain.h"
#include "Config.h"

PlayState::PlayState(Game * game) : GameState(game), m_pTerrain(new Terrain())
{
	m_pFont = new sf::Font();
	char* winDir = getenv("WinDir"); //Get the window directory
	m_pFont->loadFromFile(std::string(winDir) + "\\Fonts\\Ebrima.ttf");

	m_pPlayer = new Player("..\\resources\\player.png");

	m_pTerrain->GenerateTerrain(300, 300, Config::GetInstance().GetWindowSizeHeight(), 0, Config::GetInstance().GetWindowSizeWidth(), 50, 4);

	m_WasSpacePressed = false;
}

PlayState::~PlayState()
{
	delete m_pFont;
	delete m_pPlayer;
	delete m_pTerrain;
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
		}

		bool isSpacePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

		if (isSpacePressed && !m_WasSpacePressed)
		{
			m_pTerrain->DrawCircle(rand() % Config::GetInstance().GetWindowSizeWidth(), rand() % 300 + (Config::GetInstance().GetWindowSizeHeight() - 300), rand() % 100);
			m_WasSpacePressed = true;
		}
		else if (!isSpacePressed && m_WasSpacePressed)
		{
			m_WasSpacePressed = false;
		}
	}
}

void PlayState::Render(sf::RenderWindow * window)
{
	window->clear();
	m_pTerrain->Render(window);
	window->display();
}
