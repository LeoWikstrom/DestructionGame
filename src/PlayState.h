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
	class Text;
}

class Game;
class Player;
class Terrain;
class Enemy;
class Explosion;
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
	std::vector<Enemy*> m_Enemies;
	float m_CameraOffset;
	float m_nextTerrainUpdate;

	sf::Sprite** m_ppLifeSprites;
	sf::Texture* m_pLifeTex;

	sf::Text* m_pVisScore;

	std::vector<Explosion*> m_Explosions;

public:
	PlayState(Game* game, sf::Font* font);
	~PlayState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif