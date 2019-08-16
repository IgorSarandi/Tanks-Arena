#pragma once
#include "MotionlessObjects.h"
class Wall :
	public MotionlessObjects
{
public:
	Wall();
	~Wall();

	void SetHP(int _hp);

	int GetHP();

	std::pair<char, int> Figure() override;
private:
	int hitPoints;
};

