#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.h"
#include <vector>
#include <queue>
namespace sf
{
	class Font;
}

class Game;
class Player;
class Terrain;
class Enemy;

class PlayState : public GameState
{
private:

	sf::Font* m_pFont;
	Player* m_pPlayer;
	Terrain* m_pTerrain;
	bool m_WasSpacePressed;
	std::queue<Enemy*> m_Enemies;

public:
	PlayState(Game* game);
	~PlayState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif