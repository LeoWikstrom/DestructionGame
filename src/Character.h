#ifndef CHARACTER_H
#define CHARACTER_H
#include "Entity.h"
#include <queue>

namespace sf
{
	class Sprite;
	class Texture;
	class RenderWindow;
	template<typename T>
	class Vector2;
	typedef Vector2<int> Vector2i;
	typedef Vector2<float> Vector2f;
	class Image;
}

class Projectile;

class Character : public Entity
{
protected:
	sf::Sprite* m_pWeaponSprite;
	sf::Texture* m_pWeaponTex;
	std::queue<Projectile*> m_Projectiles;

	float m_WeaponAngle;
	int m_WalkingSpeed;
	bool m_Falling;
	bool m_Jumping;
	float m_FallingTime;

public:
	Character(const char* texturePath, const char* weaponTexturePath);
	virtual ~Character();

	virtual void Move(float dt);
	virtual bool CheckTerrainCollision(sf::Image* terrain);
	virtual void RotateWeapon(bool direction);
	virtual void SetWeaponRotation(float rotation);
	virtual void Shoot();
	virtual bool IsInAir();
	virtual bool IsExplosion();

	virtual void Update(float dt, sf::RenderWindow* window) = 0;
	virtual void Render(sf::RenderWindow* window) = 0;

};


#endif