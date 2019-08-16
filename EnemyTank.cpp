#include "EnemyTank.h"
#include <random>
#include <string>


EnemyTank::EnemyTank()
{
}


EnemyTank::~EnemyTank()
{
}

std::string EnemyTank::RandAction()
{
	///его передвижения и стрельба
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<>dis(0, 5);
	int dir = dis(gen);
	
	if (dir == 0)
		return "STOP";
	else if (dir == 1)
		return "UP";
	else if (dir == 2)
		return "DOWN";
	else if (dir == 3)
		return "LEFT";
	else if (dir == 4)
		return "RIGHT";
	else if (dir == 5)
		return "FIRE";

}