#include "PlayState.h"
#include "SFML/Graphics.hpp"
#include "Game.h"
#include "Player.h"

PlayState::PlayState(Game * game) : GameState(game)
{
	m_pFont = new sf::Font();
	char* winDir = getenv("WinDir"); //Get the window directory
	m_pFont->loadFromFile(std::string(winDir) + "\\Fonts\\Ebrima.ttf");

	m_pPlayer = new Player("..\\resources\\player.png");
}

PlayState::~PlayState()
{
	delete m_pFont;
	delete m_pPlayer;
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
	window->display();
}
