#include "Terrain.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Config.h"
#include <iostream>
Terrain::Terrain() : m_pImage(new sf::Image()), m_pTexture(new sf::Texture), m_pSprite(new sf::Sprite),
m_pSecImage(nullptr), m_pMainImage(nullptr)//, m_pSecTexture(new sf::Texture()), m_pSecSprite(new sf::Sprite())
{
}

Terrain::~Terrain()
{
	delete m_pImage;
	delete m_pTexture;
	delete m_pSprite;
	delete m_pSecImage;
	delete m_pMainImage;
	//delete m_pSecTexture;
	//delete m_pSecSprite;
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
}

void Terrain::GenSecondTerrain(unsigned int endHeight, int maxHeight, int minHeight, unsigned int maxDisplacement, unsigned int divisions)
{
	int lastImageLength = 0;
	if (m_pSecImage != nullptr)
	{
		lastImageLength = m_pImage->getSize().x;
		sf::Image* toUpdate = nullptr;

		toUpdate = m_pImage;
		m_pImage = m_pSecImage;
		m_pSecImage = toUpdate;
	}
	else
	{
		m_pSecImage = new sf::Image();
		m_pMainImage = new sf::Image();
	}


	//Get endheight of last image to set as startheight
	unsigned int startHeight = 0;
	for (int i = 0; i < m_pImage->getSize().y; i++)
	{
		if (m_pImage->getPixel(m_pImage->getSize().x - 1, i) == GROUND_COLOUR)
		{
			startHeight = Config::GetInstance().GetWindowSizeHeight() - i;
			break;
		}
	}


	//initialize midpoints
	std::vector<int> midPoints;
	midPoints.push_back(startHeight);
	RecursiveGenTerrainHelper(startHeight, endHeight, maxHeight, minHeight, m_pImage->getSize().y, divisions, maxDisplacement, &midPoints);
	midPoints.push_back(endHeight);
	m_pSecImage->create(m_pImage->getSize().x, maxHeight, SKY_COLOUR);

	int startLineHeight = 0;
	int endLineHeight = 0;
	float midPointWidth = ((float)m_pImage->getSize().x / (midPoints.size() - 1));
	int counter = 0;

	float k;
	int m;
	int x = 0;
	bool error = false;

	for (unsigned int i = 0; i < m_pImage->getSize().x; i++)
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
			m_pSecImage->setPixel(i, m_pSecImage->getSize().y - 1 - j, GROUND_COLOUR);
		}
	}

	if (counter != midPoints.size() - 1)
		std::cout << "ERROR!";

	m_pMainImage->create(m_pImage->getSize().x + m_pSecImage->getSize().x, m_pSecImage->getSize().y);
	m_pMainImage->copy(*m_pImage, 0, 0);
	m_pMainImage->copy(*m_pSecImage, m_pImage->getSize().x, 0);

	m_pTexture->loadFromImage(*m_pMainImage);
	m_pSprite->setTexture(*m_pTexture);
	m_pSprite->setTextureRect(sf::IntRect(0, 0, Config::GetInstance().GetWindowSizeWidth()*2, Config::GetInstance().GetWindowSizeHeight()));
	m_pSprite->setPosition(m_pSprite->getPosition().x + lastImageLength, 0);
	//m_pSecTexture->loadFromImage(*m_pSecImage);
	//m_pSecSprite->setTexture(*m_pSecTexture);
	//m_pSecSprite->setTextureRect(sf::IntRect(0, 0, Config::GetInstance().GetWindowSizeWidth(), Config::GetInstance().GetWindowSizeHeight()));
	//m_pSecSprite->setPosition(m_pSprite->getPosition().x + m_pImage->getSize().x, 0);
}

sf::Image& Terrain::GetTerrain()
{
	return m_pMainImage ? *m_pMainImage : *m_pImage;
}

void Terrain::Update()
{
	m_pTexture->loadFromImage(m_pMainImage ? *m_pMainImage : *m_pImage);
	m_pSprite->setTexture(*m_pTexture);
}

void Terrain::Render(sf::RenderWindow* window)
{
	window->draw(*m_pSprite);
	//window->draw(*m_pSecSprite);
}
