#ifndef ENTITY_H
#define ENTITY_H

namespace sf
{
	class Sprite;
	class Texture;
	class RenderWindow;
}

class Entity
{
protected:
	sf::Sprite* m_pSprite;
	sf::Texture* m_pTex;

public:
	Entity(const char* texturePath);
	virtual ~Entity();

	virtual void Update(float dt, sf::RenderWindow* window) = 0;
	virtual void Render(sf::RenderWindow* window) = 0;

};


#endif