#ifndef ENTITY_H
#define ENTITY_H

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

class Entity
{
protected:
	sf::Sprite* m_pSprite;
	sf::Texture* m_pTex;

	sf::Vector2i* m_pCurrentKeyFrame;
	sf::Vector2i* m_pKeyFrameSize;
	sf::Vector2i* m_pSpriteSheetSize;
	sf::Vector2f* m_pStartPosition;
	float m_AnimationSpeed;
	float m_KeyFrameDuration;
	int m_BaseKeyFrame;
	int m_AnimationStart;
	int m_AnimationLength;
	float m_SpeedX;
	float m_SpeedY;
	int m_LeftBound;
	int m_RightBound;
	int m_TopBound;
	int m_BottomBound;
	int m_BoundOffsetX;
	int m_BoundOffsetY;
	int m_OffsetBounds;
	bool m_IsOffset;

public:
	Entity(const char* texturePath);
	virtual ~Entity();

	virtual void Move(float dt);
	virtual bool CheckTerrainCollision(sf::Image* terrain);
	virtual const sf::Vector2f GetPosition();
	virtual void SetPosition(int x, int y);
	virtual void SetPosition(sf::Vector2f position);
	virtual void UpdateBounds();
	virtual void OffsetBounds();

	virtual void Update(float dt, sf::RenderWindow* window) = 0;
	virtual void Render(sf::RenderWindow* window) = 0;

};


#endif