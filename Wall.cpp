#include "Wall.h"



Wall::Wall()
{
}


Wall::~Wall()
{
}

void Wall::SetHP(int _hp)
{
	hitPoints = _hp;
}

int Wall::GetHP()
{
	return hitPoints;
}

std::pair<char, int> Wall::Figure()
{
	if(hitPoints > 1) return std::make_pair('X', 11);
	else return std::make_pair('x', 11);
}