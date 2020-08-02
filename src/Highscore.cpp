#include "Highscore.h"
#include <fstream>
#include<sstream>
Highscore::Highscore()
{
    if (!LoadScores())
    {
        m_pScores[0].name[0] = 'A';
        m_pScores[0].name[1] = 'A';
        m_pScores[0].name[2] = 'A';
        m_pScores[0].score = 50;

        m_pScores[1].name[0] = 'B';
        m_pScores[1].name[1] = 'B';
        m_pScores[1].name[2] = 'B';
        m_pScores[1].score = 40;

        m_pScores[2].name[0] = 'C';
        m_pScores[2].name[1] = 'C';
        m_pScores[2].name[2] = 'C';
        m_pScores[2].score = 30;

        m_pScores[3].name[0] = 'D';
        m_pScores[3].name[1] = 'D';
        m_pScores[3].name[2] = 'D';
        m_pScores[3].score = 20;

        m_pScores[4].name[0] = 'E';
        m_pScores[4].name[1] = 'E';
        m_pScores[4].name[2] = 'E';
        m_pScores[4].score = 10;

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

void Highscore::AddScore(const char* name, unsigned int score)
{
    if (score > GetInstance().m_pScores[4].score)
    {
        Scoring entry;
        entry.name[0] = name[0];
        entry.name[1] = name[1];
        entry.name[2] = name[2];
        entry.score = score;
        GetInstance().m_pScores[4] = entry;

        unsigned int counter = 3;

        while (counter >= 0)
        {
            if (GetInstance().m_pScores[counter].score < entry.score)
            {
                GetInstance().m_pScores[counter + 1] = GetInstance().m_pScores[counter];
                GetInstance().m_pScores[counter--] = entry;
            }
            else 
                break;
        }

        GetInstance().SaveScores();
    }
}

const char* Highscore::GetName(unsigned int entry)
{
    return GetInstance().m_pScores[entry].name;
}

unsigned int Highscore::GetScore(unsigned int entry)
{
    return GetInstance().m_pScores[entry].score;
}

unsigned int Highscore::GetNrOfEntries()
{
    return 5;
}

Highscore& Highscore::GetInstance()
{
    static Highscore instance;
    return instance;
}
