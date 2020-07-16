#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.h"
#include <vector>
namespace sf
{
	class Font;
}

class Game;
class Player;

class PlayState : public GameState
{
private:

	sf::Font* m_pFont;
	Player* m_pPlayer;

public:
	PlayState(Game* game);
	~PlayState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif