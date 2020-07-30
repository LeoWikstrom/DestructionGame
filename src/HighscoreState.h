#ifndef HIGHSCORESTATE_H
#define HIGHSCORESTATE_H
#include "GameState.h"

namespace sf
{
	class Text;
	class Font;
}

class HighscoreState : public GameState
{
private:
	sf::Text* m_pEntries;
	sf::Font* m_pFont;
public:
	HighscoreState(Game* game, sf::Font* font);
	~HighscoreState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif