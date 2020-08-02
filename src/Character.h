#ifndef CHARACTER_H
#define CHARACTER_H
#include "Entity.h"
#include <queue>
#include <vector>

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
	class RectangleShape;
}

class Projectile;
class Explosion;

class Character : public Entity
{
protected:
	sf::Sprite* m_pWeaponSprite;
	sf::Texture* m_pWeaponTex;
	std::queue<Projectile*> m_Projectiles;
	std::vector<Explosion*>* m_pExplosions;

	float m_WeaponAngle;
	int m_WalkingSpeed;
	bool m_Falling;
	bool m_Jumping;
	bool m_Exploded;
	float m_FallingTime;
	int m_Weight;
	unsigned int m_CurrentHealth;
	unsigned int m_MaxHealth;
	bool m_Invulnerable;

public:
	Character(const char* texturePath, const char* weaponTexturePath, std::vector<Explosion*>* explosions, unsigned int health = 0);
	virtual ~Character();

	virtual void Move(float dt);
	virtual bool CheckTerrainCollision(sf::Image* terrain);
	virtual void RotateWeapon(bool direction);
	virtual void SetWeaponRotation(float rotation);
	virtual void Shoot();
	virtual bool IsInAir();
	virtual bool IsExploded();
	virtual void OffsetBounds();
	virtual unsigned int GetCurrentHealth();
	virtual unsigned int GetMaxHealth();
	virtual void Update(float dt, sf::RenderWindow* window, float offset) = 0;
	virtual void Render(sf::RenderWindow* window) = 0;
	void Explode(sf::Vector2f angle);

};


#endif