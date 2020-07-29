#ifndef PLAYSTATE_H
#define PLAYSTATE_H
#include "GameState.h"
#include <vector>
#include <queue>
namespace sf
{
	class Font;
	class Sprite;
	class Texture;
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
	bool m_WasWPressed;
	bool m_WasAPressed;
	bool m_WasSPressed;
	bool m_WasDPressed;
	bool m_WasUpPressed;
	bool m_WasLeftPressed;
	bool m_WasDownPressed;
	bool m_WasRightPressed;
	std::queue<Enemy*> m_Enemies;
	float m_CameraOffset;
	float m_nextTerrainUpdate;

	sf::Sprite** m_ppLifeSprites;
	sf::Texture* m_pLifeTex;

public:
	PlayState(Game* game);
	~PlayState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif