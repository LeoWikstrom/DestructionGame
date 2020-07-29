#ifndef SCORE_H
#define SCORE_H

class Score
{
private:
	unsigned int m_DistanceScore;
	unsigned int m_KillScore;
	Score();

public:
	Score(const Score& other) = delete;
	static Score& GetInstance();

	static unsigned int GetScore();
	static void SetDistance(unsigned int dist);
	static void AddKillScore(unsigned int value);

	static void ResetScore();
};

#endif