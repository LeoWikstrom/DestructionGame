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

	sf::Image* m_pSecImage;
	sf::Image* m_pMainImage;

public:
	Terrain();
	~Terrain();

	void InitTerrain(unsigned int startHeight, unsigned int endHeight, int maxHeight, int minHeight, unsigned int length, unsigned int maxDisplacement, unsigned int divisions);
	void GenSecondTerrain(unsigned int endHeight, int maxHeight, int minHeight, unsigned int maxDisplacement, unsigned int divisions);
	sf::Image& GetTerrain();
	void Update();
	void Render(sf::RenderWindow* window);
	void Render(sf::RenderTexture* tex);
};


#endif TERRAIN_H