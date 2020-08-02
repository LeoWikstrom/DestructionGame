#include "HighscoreState.h"
#include <SFML/Graphics.hpp>
#include "Highscore.h"
#include "Config.h"
HighscoreState::HighscoreState(Game* game, sf::Font* font): GameState(game), m_pFont(font), m_pEntries(new sf::Text[5])
{
	Highscore::GetInstance();
	for (int i = 0; i < 5; i++)
	{
		std::string tmp = Highscore::GetName(i);
		tmp = tmp.substr(0, 3);
		tmp.append(": ");
		tmp.append(std::to_string(Highscore::GetScore(i)));
		m_pEntries[i].setString(tmp);
		m_pEntries[i].setFont(*m_pFont);

		m_pEntries[i].setPosition(Config::GetInstance().GetWindowSizeWidth() / 2 - m_pEntries[i].getGlobalBounds().width / 2, (Config::GetInstance().GetWindowSizeHeight() * 4 / 5) * i / 5 + (Config::GetInstance().GetWindowSizeHeight() / 10));
	}
}

HighscoreState::~HighscoreState()
{
	delete[] m_pEntries;
}

void HighscoreState::Update(float dt, sf::RenderWindow* window)
{
	sf::Event ev;

	while (window->pollEvent(ev))
	{
		if (ev.type == sf::Event::KeyPressed)
		{
			PopState();
		}

		if (ev.type == sf::Event::Closed)
		{
			PopState();
			window->close();
		}
	}
}

void HighscoreState::Render(sf::RenderWindow* window)
{
	window->clear();
	for (int i = 0; i < 5; i++)
	{
		window->draw(m_pEntries[i]);
	}
	window->display();
}
