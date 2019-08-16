#include "Tanks.h"
#include "Bullet.h"


Tanks::Tanks() : shoot(false)
{
}

Tanks::~Tanks()
{
}

void Tanks::SetHP(int _hp)
{
	hitPoints = _hp;
}

int Tanks::GetHP()
{
	return hitPoints;
}

void Tanks::Update()
{
	if (shoot)
	{
		end = sc.now();
		auto time_span = static_cast<std::chrono::duration<double>>(end - start);   // measure time span between start & end
		double d = time_span.count();

		if (d + 0.1 > 0.5)	shoot = false;
	}
}

void Tanks::Fire()
{
	if (!shoot)
	{
		Bullet* bullet = new Bullet(this);
			//в конструкторе задаем tank-parent
			//даем ему направление и скорость(ее можно переопределить в потомках)
		shoot = true;
		//ставим таймер на n миллисекунд
		//затем shoot перевести в false иначе 1 выстрел = 2 клика SPACE
		start = sc.now();
	}
}

std::pair<char, int> Tanks::Figure()
{
	if(GetDirection() == 0 || GetDirection() == 1)//stop or up
		return std::make_pair(char(30), 14);
	if (GetDirection() == 2)//down
		return std::make_pair(char(31), 14);
	if (GetDirection() == 3)//left
		return std::make_pair(char(17), 14);
	if (GetDirection() == 4)//right
		return std::make_pair(char(16), 14);
}