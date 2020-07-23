#ifndef PLAYER_H
#define PLAYER_H
#include "Entity.h"

class Player : public Entity
{
private:
	sf::Sprite* m_pCoverSprite;
	sf::Texture* m_pCoverTex;

	bool m_ShowWeapon;
public:
	Player(const char* texturePath, const char* weaponTexturePath);
	~Player();

	void SetWalkingSpeed(int speed);
	void ShowWeapon(bool show);
	bool IsWeaponOut();

	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif