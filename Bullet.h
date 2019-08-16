#pragma once
#include "MovingObjects.h"
#include "Tanks.h"

#include <vector>


class Bullet :
	public MovingObjects
{
public:
	Bullet(Tanks* parent);
	~Bullet();

	Tanks* GetParent();

	std::pair<char, int> Figure() override;

	const int cur_dir;

	std::pair<int, int> parent_coord;

	bool activate;

	static std::vector<Bullet*> storage;

private:
	void SetParent(Tanks* _parent);
	Tanks* parent = nullptr;
};

