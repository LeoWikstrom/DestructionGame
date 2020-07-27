#include "Terrain.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Config.h"
#include <iostream>
Terrain::Terrain() : m_pImage(new sf::Image()), m_pTexture(new sf::Texture), m_pSprite(new sf::Sprite), m_pRenderTex(new sf::RenderTexture)
{
}

Terrain::~Terrain()
{
	delete m_pImage;
	delete m_pTexture;
	delete m_pSprite;
	delete m_pRenderTex;
}

void RecursiveGenTerrainHelper(int startHeight, int endHeight, int maxHeight, int minHeight, unsigned int length, unsigned int divisions, unsigned int maxDisplacement, std::vector<int>* midPoints)
{
	// Calculate current height of the midpoint
	int currHeight = ((endHeight - startHeight) / 2) + startHeight;

	//calc displacement
	int disp = (rand() % maxDisplacement) - maxDisplacement / 2;
	int dispHeight = currHeight + disp;

	if (dispHeight < minHeight)
		dispHeight += maxDisplacement / 2;
	else if (dispHeight > maxHeight)
		dispHeight -= maxDisplacement / 2;

	if (divisions != 1)
	{
		RecursiveGenTerrainHelper(startHeight, dispHeight, maxHeight, minHeight, length / 2, divisions - 1, maxDisplacement, midPoints);
	}

	midPoints->push_back(dispHeight);

	if (divisions != 1)
	{
		RecursiveGenTerrainHelper(dispHeight, endHeight, maxHeight, minHeight, length / 2, divisions - 1, maxDisplacement, midPoints);
	}
}

void Terrain::InitTerrain(unsigned int startHeight, unsigned int endHeight, int maxHeight, int minHeight, unsigned int length, unsigned int maxDisplacement, unsigned int divisions)
{
	//initialize midpoints
	std::vector<int> midPoints;
	midPoints.push_back(startHeight);
	RecursiveGenTerrainHelper(startHeight, endHeight, maxHeight, minHeight, length, divisions, maxDisplacement, &midPoints);
	midPoints.push_back(endHeight);
	m_pImage->create(length, maxHeight, SKY_COLOUR);

	int startLineHeight = 0;
	int endLineHeight = 0;
	float midPointWidth = ((float)length / (midPoints.size() - 1));
	int counter = 0;

	float k;
	int m;
	int x = 0;
	bool error = false;

	for (unsigned int i = 0; i < length; i++)
	{
		if (i == (int)round((float)counter * midPointWidth))
		{
			x = 0;
			if (counter != midPoints.size() - 1)
			{
				startLineHeight = midPoints[counter];
				endLineHeight = midPoints[counter + 1];
				counter++;
			}
			else
				std::cout << "error...";

			// k*x + m = y
			k = (float)(endLineHeight - startLineHeight) / midPointWidth;

			m = startLineHeight;
		}

		unsigned int height = (k * x++ + m);

		for (unsigned int j = 0; j < height; j++)
		{
			m_pImage->setPixel(i, m_pImage->getSize().y - 1 - j, GROUND_COLOUR);
		}
	}

	if (counter != midPoints.size() - 1)
		std::cout << "ERROR!";

	m_pTexture->loadFromImage(*m_pImage);
	m_pSprite->setTexture(*m_pTexture);
	m_pSprite->setTextureRect(sf::IntRect(0, 0, Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight()));
	m_pRenderTex->create(Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight());
	m_pRenderTex->clear();
	m_pRenderTex->draw(*m_pSprite);
	m_pRenderTex->display();
	m_pSprite->setTexture(m_pRenderTex->getTexture());
}

sf::Image& Terrain::GetTerrain()
{
	return *m_pImage;
}

void Terrain::DrawCircle(int x, int y, int radius)
{
	sf::CircleShape explosion(radius);
	explosion.setFillColor(SKY_COLOUR);
	explosion.setPosition(x, y);
	m_pRenderTex->draw(explosion);

	m_pRenderTex->display();

	m_pSprite->setTexture(m_pRenderTex->getTexture());

	*m_pImage = m_pRenderTex->getTexture().copyToImage();
}

void Terrain::Update()
{
	m_pTexture->loadFromImage(*m_pImage);
	m_pSprite->setTexture(*m_pTexture);
}

void Terrain::Render(sf::RenderWindow* window)
{
	window->draw(*m_pSprite);
}
