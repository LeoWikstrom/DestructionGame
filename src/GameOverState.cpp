#include "GameOverState.h"
#include <SFML/Graphics.hpp>
#include "Score.h"
#include "Config.h"
#include "Highscore.h"
GameOverState::GameOverState(Game* game, sf::Font* font) : GameState(game), m_pFont(font), m_pScoreText(new sf::Text()), m_pInfoText(new sf::Text()), m_ppNameCharactersText(new sf::Text*[3]), m_pSpr(new sf::Sprite()), m_pTex(new sf::Texture()), m_High(false)
{
	m_pScoreText->setFont(*m_pFont);
	m_pInfoText->setFont(*m_pFont);

	if (Highscore::GetScore(Highscore::GetNrOfEntries() - 1) < Score::GetScore())
	{
		m_High = true;
		m_pTex->loadFromFile("..\\resources\\Arrows.png");
		m_pSpr->setTexture(*m_pTex);

		for (int i = 0; i < 3; i++)
		{
			m_ppNameCharactersText[i] = new sf::Text();
			m_ppNameCharactersText[i]->setFont(*m_pFont);
			m_ppNameCharactersText[i]->setString("A");
			m_ppNameCharactersText[i]->setOrigin(m_ppNameCharactersText[i]->getGlobalBounds().width / 2, m_ppNameCharactersText[i]->getGlobalBounds().height / 2);
			m_ppNameCharactersText[i]->setPosition(Config::GetInstance().GetWindowSizeWidth() / 2 - m_pScoreText->getGlobalBounds().width / 2 + i * 32, Config::GetInstance().GetWindowSizeHeight() / 2 - m_pScoreText->getGlobalBounds().height / 2);
		}
		m_ppNameCharactersText[0]->setColor(sf::Color::Blue);
		m_pInfoText->setString("Congratulations, you made a new highscore!\nPlease enter your name with the arrow keys\n");

		m_pSpr->setPosition(m_ppNameCharactersText[0]->getPosition() + sf::Vector2f(m_ppNameCharactersText[0]->getGlobalBounds().width / 2 - m_pSpr->getGlobalBounds().width / 2 , -10));
	}

	m_pScoreText->setString("Final score:\n " + std::to_string(Score::GetScore()));
	m_pInfoText->setString(m_pInfoText->getString() + "Press enter to continue");

	m_pScoreText->setPosition(Config::GetInstance().GetWindowSizeWidth() / 2 - m_pScoreText->getGlobalBounds().width / 2, Config::GetInstance().GetWindowSizeHeight() / 2 - m_pScoreText->getGlobalBounds().height * 2);
	m_pInfoText->setPosition(0, Config::GetInstance().GetWindowSizeHeight() * 2 / 3);

}

GameOverState::~GameOverState()
{
	delete m_pScoreText;
	delete m_pInfoText;
	if (m_High)
	{
		for (int i = 0; i < 3; i++)
		{
			delete m_ppNameCharactersText[i];
		}
	}
	delete[] m_ppNameCharactersText;
	delete m_pTex;
	delete m_pSpr;
}

void GameOverState::Update(float dt, sf::RenderWindow* window)
{
	if (m_High)
	{
		for (int i = 0; i < 3; i++)
		{
			m_ppNameCharactersText[i]->setOrigin(m_ppNameCharactersText[i]->getGlobalBounds().width / 2, m_ppNameCharactersText[i]->getGlobalBounds().height / 2);
			m_ppNameCharactersText[i]->setPosition(Config::GetInstance().GetWindowSizeWidth() / 2 - m_pScoreText->getGlobalBounds().width / 2 + i * 32, Config::GetInstance().GetWindowSizeHeight() / 2 - m_pScoreText->getGlobalBounds().height / 2);
		}

		m_pSpr->setPosition(m_ppNameCharactersText[m_characterCounter]->getPosition() + sf::Vector2f(-m_pSpr->getGlobalBounds().width / 2, -10 - m_ppNameCharactersText[m_characterCounter]->getGlobalBounds().height / 2));
	}

	if (window->hasFocus())
	{
		sf::Event ev;
		while (window->pollEvent(ev))
		{
			if (ev.type == sf::Event::KeyPressed)
			{
				if (m_High)
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
					{
						std::string text = m_ppNameCharactersText[m_characterCounter]->getString();
						text[0] += 1;
						if (text[0] == 'Z' + 1)
						{
							text[0] = 'A';
						}
						m_ppNameCharactersText[m_characterCounter]->setString(text);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
					{
						std::string text = m_ppNameCharactersText[m_characterCounter]->getString();
						text[0] -= 1;
						if (text[0] == 'A' - 1)
						{
							text[0] = 'Z';
						}
						m_ppNameCharactersText[m_characterCounter]->setString(text);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
					{
						m_ppNameCharactersText[m_characterCounter]->setColor(sf::Color::White);
						m_characterCounter = m_characterCounter - 1 < 0 ? 2 : m_characterCounter - 1;
						m_ppNameCharactersText[m_characterCounter]->setColor(sf::Color::Blue);
					}
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
					{
						m_ppNameCharactersText[m_characterCounter]->setColor(sf::Color::White);
						m_characterCounter = (m_characterCounter + 1) % 3;
						m_ppNameCharactersText[m_characterCounter]->setColor(sf::Color::Blue);
					}

					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					{
						std::string tmp;
						for (int i = 0; i < 3; i++)
						{
							tmp.push_back(m_ppNameCharactersText[i]->getString()[0]);
						}
						Highscore::AddScore(tmp.c_str(), Score::GetScore());
						PopState();
					}
				}
				else
				{
					if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
					{
						PopState();
					}
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
	if (m_High)
	{
		for (int i = 0; i < 3; i++)
		{
			window->draw(*m_ppNameCharactersText[i]);
		}
		window->draw(*m_pSpr);
	}
	window->display();
}
