#include "MovingObjects.h"



MovingObjects::MovingObjects()
{
	directions = { {"STOP", 0}, {"UP", 1}, {"DOWN", 2}, {"LEFT", 3}, {"RIGHT", 4} };
	current_direction = "STOP";
}

void MovingObjects::SetDirection(std::string _current_direction)
{
	current_direction = _current_direction;
}

int MovingObjects::GetDirection()
{
	return directions[current_direction];
}

MovingObjects::~MovingObjects()
{
}

