#include "Score.h"

Score::Score(): m_DistanceScore(0), m_KillScore(0)
{
}

Score& Score::GetInstance()
{
    static Score instance;
    return instance;
}

unsigned int Score::GetScore()
{
    return GetInstance().m_DistanceScore + GetInstance().m_KillScore;
}

void Score::SetDistance(unsigned int dist)
{
    GetInstance().m_DistanceScore = GetInstance().m_DistanceScore < dist ? dist : GetInstance().m_DistanceScore;
}

void Score::AddKillScore(unsigned int value)
{
    GetInstance().m_KillScore += value;
}

void Score::ResetScore()
{
    GetInstance().m_DistanceScore = 0;
    GetInstance().m_KillScore = 0;
}
