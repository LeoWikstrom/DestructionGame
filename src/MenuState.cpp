#include "MenuState.h"
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "PlayState.h"
#include "Score.h"
const int nrOfAlternatives = 2;

MenuState::MenuState(Game* game) : GameState(game), m_pFont(new sf::Font()), m_ppTexts(new sf::Text*[nrOfAlternatives]), m_MenuAlternative(0), m_KeyReleased(true)
{
	char* winDir = getenv("WinDir"); //Get the window directory
	m_pFont->loadFromFile(std::string(winDir) + "\\Fonts\\Ebrima.ttf");

	for (int i = 0; i < nrOfAlternatives; i++)
	{
		m_ppTexts[i] = new sf::Text();
		m_ppTexts[i]->setFont(*m_pFont);

		m_ppTexts[i]->setPosition(20, Config::GetInstance().GetWindowSizeHeight() / 3 + 50 * i);
	}

	m_ppTexts[0]->setString("Start");

	m_ppTexts[nrOfAlternatives - 1]->setString("Exit");

	m_ppTexts[0]->setFillColor(sf::Color::Blue);
}

MenuState::~MenuState()
{
	delete m_pFont;
	for (int i = 0; i < nrOfAlternatives; i++)
	{
		delete m_ppTexts[i];
	}
	delete[] m_ppTexts;
}

void MenuState::Update(float dt, sf::RenderWindow* window)
{
	if (window->hasFocus())
	{
		sf::Event ev;

		while (window->pollEvent(ev))
		{
			if (ev.type == sf::Event::Closed)
			{
				PopState();
				window->close();
			}

			if (ev.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && m_KeyReleased)
				{
					switch (m_MenuAlternative)
					{
					case 0:
						Score::ResetScore();
						ChangeState(new PlayState(m_pGame, m_pFont));
						break;
					case (nrOfAlternatives - 1):
						PopState();
						window->close();
						break;
					default:
						break;
					}
				}

				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) && m_KeyReleased)
				{
					m_ppTexts[m_MenuAlternative--]->setFillColor(sf::Color::White);
					if (m_MenuAlternative == -1)
					{
						m_MenuAlternative = nrOfAlternatives - 1;
					}
					m_ppTexts[m_MenuAlternative]->setFillColor(sf::Color::Blue);

					m_KeyReleased = false;
				}

				if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) && m_KeyReleased)
				{
					m_ppTexts[m_MenuAlternative++]->setFillColor(sf::Color::White);
					m_MenuAlternative %= nrOfAlternatives;
					m_ppTexts[m_MenuAlternative]->setFillColor(sf::Color::Blue);

					m_KeyReleased = false;
				}
			}
			else
				m_KeyReleased = true;
		}
	}
}

void MenuState::Render(sf::RenderWindow* window)
{
	window->clear();
	for (int i = 0; i < nrOfAlternatives; i++)
	{
		window->draw(*m_ppTexts[i]);
	}
	window->display();
}
