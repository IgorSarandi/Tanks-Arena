#include "MainTank.h"
#include <string>

int MainTank::health = 0;
int MainTank::score = 0;

MainTank::~MainTank()
{
}

MainTank& MainTank::getSingleton()
{
	static MainTank singleton;
	return singleton;
}

std::pair<char, int> MainTank::Figure()
{
	if (GetDirection() == 0 || GetDirection() == 1)//stop or up
		return std::make_pair(char(30), 15);
	if (GetDirection() == 2)//down
		return std::make_pair(char(31), 15);
	if (GetDirection() == 3)//left
		return std::make_pair(char(17), 15);
	if (GetDirection() == 4)//right
		return std::make_pair(char(16), 15);
}

void MainTank::IncreaseScore()
{
	score++;
}

int MainTank::GetScore()
{
	return score * 100;
}

void MainTank::SetHealth(int _hp)
{
	health = _hp;
}

std::string MainTank::GetHealth()
{
	char ch = char(3);
	std::string s;
	if (health > 0)
	{
		for (int i = 0; i < health; i++)
		{
			s.push_back(ch);
			s.push_back(' ');
		}
	}
	return s;
}