#ifndef EXPLOSION_H
#define EXPLOSION_H

namespace sf
{
	class Texture;
	class Sprite;
	class RenderWindow;
	class CircleShape;
	class Image;
}

class Character;
class Explosion
{
private:
	float m_Rad;
	float m_Ttl;
	sf::Texture* m_pTex;
	sf::Sprite* m_pSpr;
	bool m_Exploded;
	bool m_New;
public:
	Explosion(float x, float y, float radius);
	~Explosion();
	void Update(float dt, sf::RenderWindow* window);
	void Render(sf::RenderWindow* window);
	bool CheckCollision(Character* character);
	bool ShouldLive();
	void ClearTerrain(sf::Image* terrain, float offset);
};


#endif