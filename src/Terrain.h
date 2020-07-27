#ifndef TERRAIN_H
#define TERRAIN_H
namespace sf
{
	class Image;
	class Texture;
	class Sprite;
	class RenderWindow;
	class RenderTexture;
}

class Terrain
{
private:
	sf::Image* m_pImage;
	sf::Texture* m_pTexture;
	sf::Sprite* m_pSprite;
	sf::RenderTexture* m_pRenderTex;

public:
	Terrain();
	~Terrain();

	void InitTerrain(unsigned int startHeight, unsigned int endHeight, int maxHeight, int minHeight, unsigned int length, unsigned int maxDisplacement, unsigned int divisions);
	sf::Image& GetTerrain();
	void DrawCircle(int x, int y, int radius);
	void Update();
	void Render(sf::RenderWindow* window);
};


#endif TERRAIN_H