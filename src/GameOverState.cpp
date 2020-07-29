#include "GameOverState.h"
#include <SFML/Graphics.hpp>
#include "Score.h"
#include "Config.h"
GameOverState::GameOverState(Game* game, sf::Font* font): GameState(game), m_pFont(font), m_pScoreText(new sf::Text()), m_pInfoText(new sf::Text())
{
	m_pScoreText->setFont(*m_pFont);
	m_pInfoText->setFont(*m_pFont);

	m_pScoreText->setString("Final score:\n " + std::to_string(Score::GetScore()));
	m_pInfoText->setString("Press enter to continue");

	m_pScoreText->setPosition(Config::GetInstance().GetWindowSizeWidth() / 2 - m_pScoreText->getGlobalBounds().width / 2, Config::GetInstance().GetWindowSizeHeight() / 2 - m_pScoreText->getGlobalBounds().height / 2);
	m_pInfoText->setPosition(0, Config::GetInstance().GetWindowSizeHeight() * 2 / 3);
}

GameOverState::~GameOverState()
{
	delete m_pScoreText;
	delete m_pInfoText;
}

void GameOverState::Update(float dt, sf::RenderWindow* window)
{
	if (window->hasFocus())
	{
		sf::Event ev;

		while (window->pollEvent(ev))
		{
			if (ev.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					PopState();
				}
			}
		}
	}
}

void GameOverState::Render(sf::RenderWindow* window)
{
	window->clear();
	window->draw(*m_pScoreText);
	window->draw(*m_pInfoText);
	window->display();
}
