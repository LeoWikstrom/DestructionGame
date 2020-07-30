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
	Highscore();
public:
	Highscore(const Highscore& other) = delete;
	~Highscore();

	bool LoadScores();
	void SaveScores();

	static bool EvaluateScore(unsigned int score);
	static void AddScore(Scoring entry);
	static void AddScore(const char* name, unsigned int score);

	static const char* GetName(unsigned int entry);
	static unsigned int GetScore(unsigned int entry);
	static unsigned int GetNrOfEntries();

	static Highscore& GetInstance();
};

#endif