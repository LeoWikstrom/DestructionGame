#ifndef HIGHSCORE_H
#define HIGHSCORE_H

struct Scoring
{
	char name[3];
	unsigned int score;
};

class Highscore
{
private:
	Scoring m_pScores[5];
public:
	Highscore();
	~Highscore();

	bool LoadScores();
	void SaveScores();

	bool EvaluateScore(unsigned int score);
	void AddScore(Scoring entry);
	void AddScore(const char* name, unsigned int score);
};

#endif