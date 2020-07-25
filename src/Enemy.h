#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"

class Enemy : public Character
{
private:
	int m_DetectionRadius;
	int m_Accuracy;
public:
	Enemy(const char* texturePath, const char* weaponTexturePath, int detectionRadius, int accuracy);
	~Enemy();

	void CheckForPlayer(int playerX, int playerY);
	void AimForPlayer(int playerX, int playerY);
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif