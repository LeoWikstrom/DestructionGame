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

class Projectile : public Entity
{
protected:
	int m_Damage;
	int m_Power;
	int m_Direction;
	bool m_Shooting;
	std::vector<sf::Vector2f*> m_Explosions;
	sf::Image* m_pTerrainImage;

public:
	Projectile(const char* texturePath, int damage, int power);
	virtual ~Projectile();

	void SetRotation(float angle);
	void SetDirection(int direction);
	void Shoot();
	bool IsShooting();
	void UpdateExplosion(float dt);
	bool IsExplosion();
	virtual void Move(float dt);
	virtual bool CheckTerrainCollision(sf::Image* terrain);
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);

};


#endif