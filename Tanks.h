#pragma once
#include "MovingObjects.h"

#include <chrono>

class Tanks :
	public MovingObjects
{
public:
	Tanks();

	virtual ~Tanks();

	void SetHP(int _hp);

	int GetHP();

	void Fire();

	virtual std::pair<char, int> Figure() override;

	void Update();
private:
	bool shoot;
	int hitPoints;

	std::chrono::steady_clock sc;
	std::chrono::time_point<std::chrono::steady_clock> start;
	std::chrono::time_point<std::chrono::steady_clock> end;
};

