#include "Highscore.h"
#include <fstream>
#include<sstream>
Highscore::Highscore()
{
    if (!LoadScores())
    {
        m_pScores[0].name[0] = 'a';
        m_pScores[0].name[1] = 'a';
        m_pScores[0].name[2] = 'a';
        m_pScores[0].score = 500;

        m_pScores[1].name[0] = 'b';
        m_pScores[1].name[1] = 'b';
        m_pScores[1].name[2] = 'b';
        m_pScores[1].score = 400;

        m_pScores[2].name[0] = 'c';
        m_pScores[2].name[1] = 'c';
        m_pScores[2].name[2] = 'c';
        m_pScores[2].score = 300;

        m_pScores[3].name[0] = 'd';
        m_pScores[3].name[1] = 'd';
        m_pScores[3].name[2] = 'd';
        m_pScores[3].score = 200;

        m_pScores[4].name[0] = 'e';
        m_pScores[4].name[1] = 'e';
        m_pScores[4].name[2] = 'e';
        m_pScores[4].score = 100;

        SaveScores();
        LoadScores();
    }
}

Highscore::~Highscore()
{
}

bool Highscore::LoadScores()
{
    std::ifstream file;
    file.open("..\\resources\\Scoring.bin", std::ios::in | std::ios::binary);

    if (!file.is_open())
    {
        //Error
        return false;
    }

    bool isName = true;
    unsigned int count = 0;


    while (count < 5)
    {
        //alternate between names and scores.
        if (isName)
        {
            file.read(m_pScores[count].name, sizeof(char) * 3);
            isName = false;
        }
        else
        {
            file.read(reinterpret_cast<char*>(&m_pScores[count++].score), sizeof(unsigned int));
            isName = true;
        }
    }
    file.close();
    return true;
}

void Highscore::SaveScores()
{

    std::ofstream file;
    file.open("..\\resources\\Scoring.bin", std::ios::out | std::ios::binary);

    if (!file.is_open())
    {
        //Error
        return;
    }

    bool isName = true;
    unsigned int count = 0;


    while (count < 5)
    {
        //alternate between names and scores.
        if (isName)
        {
            file.write(m_pScores[count].name, sizeof(char) * 3);
            isName = false;
        }
        else
        {
            file.write(reinterpret_cast<char*>(&m_pScores[count++].score), sizeof(unsigned int));
            isName = true;
        }
    }
    file.close();
}

bool Highscore::EvaluateScore(unsigned int score)
{
    return false;
}

void Highscore::AddScore(Scoring entry)
{
}

void Highscore::AddScore(const char* name, unsigned int score)
{
}
