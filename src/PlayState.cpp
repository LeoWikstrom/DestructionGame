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

	m_pTerrain->GenerateTerrain(100, 100, Config::GetInstance().GetWindowSizeHeight(), 0, Config::GetInstance().GetWindowSizeWidth(), 10, 4);

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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
		}
	}
}

void PlayState::Render(sf::RenderWindow * window)
{
	window->clear();
	m_pTerrain->Render(window);
	window->display();
}
