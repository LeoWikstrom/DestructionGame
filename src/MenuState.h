#ifndef MENUSTATE_H
#define MENUSTATE_H
#include "GameState.h"
#include <vector>

namespace sf
{
	class Font;
	class Text;
	class Texture;
	class Sprite;
}
class Highscore;

class MenuState : public GameState
{
private:
	sf::Font* m_pFont;
	sf::Text** m_ppTexts;
	unsigned int m_MenuAlternative;
	bool m_KeyReleased;
	std::vector<sf::Texture*> m_EnemyTex;
	std::vector<sf::Sprite*> m_EnemySprite;
	std::vector<float> m_KeyFrameDuration;
	std::vector<int> m_CurrentKeyFrameX;
	std::vector<int> m_CurrentKeyFrameY;
	std::vector<bool> m_EnemyDirection;
	std::vector<int> m_EnemySpeed;
	int m_KeyFrameSize;
	int m_AnimationLength;
	float m_AnimationSpeed;
public:
	MenuState(Game* game);
	~MenuState();
	virtual void Update(float dt, sf::RenderWindow* window);
	virtual void Render(sf::RenderWindow* window);
};

#endif