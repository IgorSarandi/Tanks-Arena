#pragma once
#include "Objects.h"

#include <string>
#include <map>

class MovingObjects :
	public Objects
	///tanks, bullets
{
public:
	MovingObjects();
	virtual ~MovingObjects();

	void SetDirection(std::string _current_direction);
	int GetDirection();

protected:
	std::map<std::string, int> directions;

private:
	std::string current_direction;
};

