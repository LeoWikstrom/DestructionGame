#include "GameState.h"

namespace sf
{
	class Font;
	class Text;
}


class GameOverState : public GameState
{
private:
	sf::Font* m_pFont;
	sf::Text* m_pScoreText;
	sf::Text* m_pInfoText;
public:
	GameOverState(Game* game, sf::Font* font);
	~GameOverState();

	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};