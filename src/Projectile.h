#ifndef PROJECTILE_H
#define PROJECTILE_H
#include "Entity.h"
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
}

class Explosion;

class Projectile : public Entity
{
protected:
	int m_Damage;
	int m_Power;
	int m_Direction;
	bool m_Shooting;
	//std::vector<sf::Vector2f*> m_Explosions;
	std::vector<Explosion*>* m_pExplosions;
	sf::Image* m_pTerrainImage;

public:
	Projectile(const char* texturePath, std::vector<Explosion*>* explosions, int damage, int power);
	virtual ~Projectile();

	void SetRotation(float angle);
	void SetDirection(int direction);
	void Shoot();
	bool IsShooting();
	virtual void Move(float dt);
	virtual bool CheckTerrainCollision(sf::Image* terrain);
	virtual void Update(float dt, sf::RenderWindow* window, float offset);
	virtual void Render(sf::RenderWindow* window);

};


#endif