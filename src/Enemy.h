#ifndef ENEMY_H
#define ENEMY_H
#include "Character.h"

class Enemy : public Character
{
private:
	int m_DetectionRadius;
	int m_Accuracy;
	int m_FullHealth;
	bool m_RotateDown;
	float m_LastDistance;
	float m_SecondLastDistance;
	float m_LastAngle;
	bool m_Shoot;
public:
	Enemy(const char* texturePath, const char* weaponTexturePath, int detectionRadius, int accuracy, int health);
	~Enemy();

	void CheckForPlayer(int playerX, int playerY);
	void AimForPlayer(int playerX, int playerY);
	virtual void RotateWeapon(bool direction);
	virtual void Update(float dt, sf::RenderWindow* window, float offset);
	virtual void Render(sf::RenderWindow* window);
};

#endif