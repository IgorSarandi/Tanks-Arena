#pragma once
#include "Tanks.h"
class EnemyTank :
	public Tanks
{
public:
	EnemyTank();
	~EnemyTank();

	std::string RandAction();
};

