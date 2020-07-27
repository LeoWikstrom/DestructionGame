#include "Game.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Config.h"
#include "PlayState.h"

void Game::ChangeState(GameState* state)
{
	m_pState = state;
}

Game::Game()
{
	m_pWindow = new sf::RenderWindow(sf::VideoMode(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight()), "Destruction Game");
	m_pState = new PlayState(this);

	sf::Image icon;
	icon.loadFromFile("..\\resources\\player_icon.png");
	m_pWindow->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
	m_pWindow->setPosition(sf::Vector2i(400 / SCALE, 0));
}

Game::~Game()
{
	delete m_pWindow;
	delete m_pState;
}

void Game::Run()
{
	sf::Clock clock;
	while (m_pWindow->isOpen())
	{
		m_pState->Update(clock.restart().asSeconds(), m_pWindow);
		m_pState->Render(m_pWindow);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_pWindow->close();
		}
	}

	while (m_pState)
		m_pState->PopState();
}
