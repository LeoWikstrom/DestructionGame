#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "GameState.h"

namespace sf
{
	class Font;
	class Text;
}
class Highscore;

class MenuState : public GameState
{
private:
	sf::Font* m_pFont;
	sf::Text** m_ppTexts;
	unsigned int m_MenuAlternative;
	bool m_KeyReleased;
public:
	MenuState(Game* game);
	~MenuState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif