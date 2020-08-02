#include "GameState.h"

namespace sf
{
	class Font;
	class Text;
	class Texture;
	class Sprite;
}


class GameOverState : public GameState
{
private:
	sf::Font* m_pFont;
	sf::Text* m_pScoreText;
	sf::Text* m_pInfoText;
	sf::Text** m_ppNameCharactersText;
	sf::Texture* m_pTex;
	sf::Sprite* m_pSpr;
	bool m_High;
	int m_characterCounter = 0;
public:
	GameOverState(Game* game, sf::Font* font);
	~GameOverState();

	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};