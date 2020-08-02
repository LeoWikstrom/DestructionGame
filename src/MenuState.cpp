#include "MenuState.h"
#include <SFML/Graphics.hpp>
#include "Config.h"
#include "PlayState.h"
#include "Score.h"
#include "HighscoreState.h"
#include "Enemy.h"
const int nrOfAlternatives = 3;

MenuState::MenuState(Game* game) : GameState(game), m_pFont(new sf::Font()), m_ppTexts(new sf::Text*[nrOfAlternatives]), m_MenuAlternative(0), m_KeyReleased(true)
{
	char* winDir = getenv("WinDir"); //Get the window directory
	m_pFont->loadFromFile(std::string(winDir) + "\\Fonts\\Ebrima.ttf");

	for (int i = 0; i < nrOfAlternatives; i++)
	{
		m_ppTexts[i] = new sf::Text();
		m_ppTexts[i]->setFont(*m_pFont);
		m_ppTexts[i]->setScale(SCALE, SCALE);

		m_ppTexts[i]->setPosition(20, Config::GetInstance().GetWindowSizeHeight() / 3 + 50 * i * SCALE);
	}

	m_ppTexts[0]->setString("Start");
	m_ppTexts[1]->setString("Highscore");
	m_ppTexts[nrOfAlternatives - 1]->setString("Exit");

	m_ppTexts[0]->setFillColor(sf::Color::Blue);

	m_KeyFrameSize = 16;
	m_AnimationLength = 4;
	m_AnimationSpeed = 0.15f;
	for (int i = 0; i < 12; ++i)
	{
		m_EnemyTex.push_back(new sf::Texture);
		m_EnemySprite.push_back(new sf::Sprite);
		char path[] = "..\\resources\\blobert1.png";
		path[20] = (i % 3) + 49;
		m_EnemyTex[i]->loadFromFile(path);
		m_EnemySprite[i]->setTexture(*m_EnemyTex[i]);
		m_EnemyDirection.push_back(rand() % 2);
		m_EnemySprite[i]->setScale((m_EnemyDirection[i] * 2 - 1) * SCALE, SCALE);
		m_EnemySprite[i]->setPosition(rand() % (Config::GetInstance().GetWindowSizeWidth() - m_KeyFrameSize * SCALE), rand() % (Config::GetInstance().GetWindowSizeHeight() - m_KeyFrameSize * SCALE));
		m_CurrentKeyFrameX.push_back(rand() % m_AnimationLength);
		m_CurrentKeyFrameY.push_back(0);
		m_EnemySprite[i]->setTextureRect(sf::IntRect(m_CurrentKeyFrameX[i] * m_KeyFrameSize, 0, m_KeyFrameSize, m_KeyFrameSize));
		m_KeyFrameDuration.push_back(0.f);
		m_EnemySpeed.push_back(rand() % 10 + 20);
	}
}

MenuState::~MenuState()
{
	delete m_pFont;
	for (int i = 0; i < nrOfAlternatives; i++)
	{
		delete m_ppTexts[i];
	}
	delete[] m_ppTexts;

	for (int i = 0; i < m_EnemySprite.size(); ++i)
	{
		delete m_EnemySprite[i];
		delete m_EnemyTex[i];
	}
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
					case 1:
						ChangeState(new HighscoreState(m_pGame, m_pFont));
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
			{
				m_KeyReleased = true;
			}
		}

		for (int i = 0; i < m_EnemySprite.size(); i++)
		{
			m_KeyFrameDuration[i] += dt;
			if (m_KeyFrameDuration[i] > m_AnimationSpeed)
			{
				m_CurrentKeyFrameX[i] = ((m_CurrentKeyFrameX[i] + 1) % m_AnimationLength);
				m_EnemySprite[i]->setTextureRect(sf::IntRect(m_CurrentKeyFrameX[i] * m_KeyFrameSize, 0, m_KeyFrameSize, m_KeyFrameSize));

				m_KeyFrameDuration[i] = 0;
			}

			m_EnemySprite[i]->move(-m_EnemySpeed[i] * SCALE * dt * (m_EnemyDirection[i] * 2 - 1), 0);

			if (m_EnemySprite[i]->getPosition().x < -m_KeyFrameSize * SCALE)
			{
				m_EnemySprite[i]->move(Config::GetInstance().GetWindowSizeWidth() + m_KeyFrameSize * SCALE,0);
			}
			else if (m_EnemySprite[i]->getPosition().x > Config::GetInstance().GetWindowSizeWidth() + m_KeyFrameSize * SCALE)
			{
				m_EnemySprite[i]->move(-(Config::GetInstance().GetWindowSizeWidth() + m_KeyFrameSize * SCALE), 0);
			}
		}
	}
}

void MenuState::Render(sf::RenderWindow* window)
{
	window->clear();

	for (int i = 0; i < m_EnemySprite.size(); ++i)
	{
		window->draw(*m_EnemySprite[i]);
	}
	for (int i = 0; i < nrOfAlternatives; i++)
	{
		window->draw(*m_ppTexts[i]);
	}
	window->display();
}
