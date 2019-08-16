#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include <vector>
#include <conio.h>
#include <string>
#include <algorithm>//for std::transform && std::find
#include <windows.h>
#include <exception>
#include <typeinfo>
#include <stdexcept>

#include "Border.h"
#include "Bullet.h"
#include "EnemyTank.h"
#include "Field.h"
#include "MainTank.h"
#include "MotionlessObjects.h"
#include "MovingObjects.h"
#include "Objects.h"
#include "Tanks.h"
#include "Wall.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::pair;
using std::vector;
using std::random_device;
using std::mt19937;


void Menu()
{
	SetConsoleTitle("Tanks ARENA");
	cout << "-----------------------------------------------" << endl;
	cout << "		 TanKs ARENA" << endl;
	cout << "-----------------------------------------------" << endl;

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));//1.5 seconds
}

pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> Initialize()
{
	int x = 60;
	int y = 24;
	int total_border = (x * 2) + (y * 2) - 4;
	int total_fields = (x - 2) * (y - 2);
	
	vector<Objects*> _borders;
	vector<Objects*> _fields;
	vector<Objects*> _walls;
	vector<Objects*> _main_tanks;
	vector<Objects*> _bullets;
	vector<Objects*> _enemy_tanks;

	vector<vector<Objects*>> _objects;
	vector<vector<pair<char, int>>> table;
	///char is figure, int is color

	table.resize(y);
	for (int i = 0; i < (int)table.size(); i++)
		table[i].resize(x);

	Border* border = new Border[total_border]();
	Field* field = new Field[total_fields]();

	_borders.reserve(total_border);
	_fields.reserve(total_fields);

	//std::copy(border[0], border[total_border - 1], std::back_inserter(objects));
	//std::copy(field[0], field[total_fields - 1], std::back_inserter(objects));

	int temp1 = 0;
	int temp2 = 0;

	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			if ((j == 0 || j == y - 1) ||
				(i == 0 && j > 0 && j < y - 1) ||
				(i == x - 1 && j > 0 && j < y - 1))
			{
				border[temp1].SetPosition(j, i);
				table[j][i] = border[temp1].Figure();

				_borders.push_back(&border[temp1]);
				
				temp1++;
			}
			else
			{
				field[temp2].SetPosition(i, j);
				table[j][i] = field[temp2].Figure();

				_fields.push_back(&field[temp2]);

				temp2++;
			}
		}
	}
	int total = x * y;
	vector <int> sequence;
	for (int i = x; i < total - x; i++)
	{
		if((i % x > 0) && (i % x < x - 1)) sequence.push_back(i);
	}
	random_device rd;
	mt19937 g(rd());
	shuffle(sequence.begin(), sequence.end(), g);
	copy(sequence.begin(), sequence.end(), sequence.begin());


	Wall* wall = new Wall[20]();//пусть будет 20 стенок
	EnemyTank* enemy_tank = new EnemyTank[5]();//пусть будет 5 танков
	_walls.reserve(20);//walls
	_main_tanks.reserve(1);//main_tank
	_enemy_tanks.reserve(5);//enemy_tanks

	MainTank& main_tank = MainTank::getSingleton();

	for (int i = 0; i < (int)sequence.size(); i++)
	{
		int k = sequence[i] / x;
		int m = sequence[i] - x * k;
		if (i < 20)
		{
			wall[i].SetPosition(m, k);
			wall[i].SetHP(2);
			table[k][m] = wall[i].Figure();

			_walls.push_back(&wall[i]);
		}
		else if (i == 20)
		{
			main_tank.SetPosition(m, k);
			main_tank.SetHP(3);
			main_tank.SetHealth(main_tank.GetHP());
			table[k][m] = main_tank.Figure();

			_main_tanks.push_back(&main_tank);
		}
		else if (i < 26)
		{
			int ii = i - 21;//правильный индекс для доступа к обьектам
			enemy_tank[ii].SetPosition(m, k);
			enemy_tank[ii].SetHP(2);
			table[k][m] = enemy_tank[ii].Figure();

			_enemy_tanks.push_back(&enemy_tank[ii]);
		}
		else break;
	}

	_objects.push_back(_borders);
	_objects.push_back(_fields);
	_objects.push_back(_walls);
	_objects.push_back(_main_tanks);
	_objects.push_back(_bullets);
	_objects.push_back(_enemy_tanks);

	pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> match;
	match.first = _objects;
	match.second = table;

	return match;
}

string Input()
{
	if (_kbhit())
	{
		int key_code = _getch();
		switch (key_code)
		{
		case 87:// is W
		case 119:// is w
			//cout << key_code;
			return "UP";
		case 65:// is A
		case 97:// is a
			//cout << key_code;
			return "LEFT";
		case 83:// is S
		case 115:// is s
			//cout << key_code;
			return "DOWN";
		case 68:// is D
		case 100:// is d
			//cout << key_code;
			return "RIGHT";
		case 32:// is SPACE
			//cout << key_code;
			return "FIRE";
		case 27:// is End button
			//cout << key_code;
			return "END";
		case 80:// is P
		case 112:// is p
			//cout << key_code;
			return "PAUSE";
		default:
			return "NULL";
		}
	}
	return "NULL";
}

template <typename From, typename To>
struct caster
{
	To* operator()(From* p) { return dynamic_cast<To*>(p); }
};

void Logic(pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>>& objects, string key = "NULL")
{
	vector<Border*> _borders;
	vector<Field*> _fields;
	vector<Wall*> _walls;
	vector<Bullet*> _bullets;
	vector<EnemyTank*> _enemy_tanks;


	std::transform(objects.first[0].begin(), objects.first[0].end(), std::back_inserter(_borders),
		caster<Objects, Border>());
	std::transform(objects.first[1].begin(), objects.first[1].end(), std::back_inserter(_fields),
		caster<Objects, Field>());
	std::transform(objects.first[2].begin(), objects.first[2].end(), std::back_inserter(_walls),
		caster<Objects, Wall>());
	
	MainTank* _MTank = nullptr;
	try
	{
		if (objects.first[3].size() == 0) 
			throw "WTF? I'm dead";
		_MTank = dynamic_cast<MainTank*>(objects.first[3][0]);
	}
	catch (const char* msg)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));//0.5 seconds
		system("cls"); 
		std::cerr << msg << endl;
		std::cerr << "Your SCORE is: " << MainTank::GetScore() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));//0.5 seconds
		system("pause");
		quick_exit(1);
	}

	std::transform(objects.first[4].begin(), objects.first[4].end(), std::back_inserter(_bullets),
		caster<Objects, Bullet>());
	std::transform(objects.first[5].begin(), objects.first[5].end(), std::back_inserter(_enemy_tanks),
		caster<Objects, EnemyTank>());

	try
	{
		if (objects.first[5].size() == 0)
			throw "CONDRATULATIONS! YOU WIN!!!";
		_MTank = dynamic_cast<MainTank*>(objects.first[3][0]);
	}
	catch (const char* msg)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));//0.5 seconds
		system("cls");
		std::cerr << msg << endl;
		std::cerr << "Your SCORE is: " << MainTank::GetScore() << endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));//0.5 seconds
		system("pause");
		quick_exit(0);
	}

	vector<Tanks*> _tanks;
	_tanks.push_back(_MTank);
	for (auto a : _enemy_tanks)
	{
		_tanks.push_back(a);
	}

	for (auto a : _tanks)
	{
		a->Update();
	}
	
	if (key == "UP")
	{
		_MTank->SetDirection(key);
		int k = _MTank->GetPosition().second;
		int m = _MTank->GetPosition().first;

		if (objects.second[k - 1][m] == (*_fields.begin())->Figure())
		{
			k = _MTank->GetPosition().second - 1;
			m = _MTank->GetPosition().first;
			_MTank->SetPosition(m, k);
			objects.second[k][m] = _MTank->Figure();
			objects.second[k + 1][m] = (*_fields.begin())->Figure();
		}
		else
		{
			objects.second[k][m] = _MTank->Figure();
		}
	}
	else if (key == "LEFT")
	{
		_MTank->SetDirection(key);
		int k = _MTank->GetPosition().second;
		int m = _MTank->GetPosition().first;

		if (objects.second[k][m - 1] == (*_fields.begin())->Figure())
		{
			k = _MTank->GetPosition().second;
			m = _MTank->GetPosition().first - 1;
			_MTank->SetPosition(m, k);
			objects.second[k][m] = _MTank->Figure();
			objects.second[k][m + 1] = (*_fields.begin())->Figure();
		}
		else
			objects.second[k][m] = _MTank->Figure();
	}
	else if (key == "DOWN")
	{
		_MTank->SetDirection(key);
		int k = _MTank->GetPosition().second;
		int m = _MTank->GetPosition().first;

		if (objects.second[k + 1][m] == (*_fields.begin())->Figure())
		{
			k = _MTank->GetPosition().second + 1;
			m = _MTank->GetPosition().first;
			_MTank->SetPosition(m, k);
			objects.second[k][m] = _MTank->Figure();
			objects.second[k - 1][m] = (*_fields.begin())->Figure();
		}
		else
			objects.second[k][m] = _MTank->Figure();
	}
	else if (key == "RIGHT")
	{
		_MTank->SetDirection(key);
		int k = _MTank->GetPosition().second;
		int m = _MTank->GetPosition().first;

		if (objects.second[k][m + 1] == (*_fields.begin())->Figure())
		{
			k = _MTank->GetPosition().second;
			m = _MTank->GetPosition().first + 1;
			_MTank->SetPosition(m, k);
			objects.second[k][m] = _MTank->Figure();
			objects.second[k][m - 1] = (*_fields.begin())->Figure();
		}
		else
			objects.second[k][m] = _MTank->Figure();
	}
	else if (key == "FIRE")
	{
		_MTank->Fire();
	}
	else if (key == "END")
	{
		quick_exit(0);
	}
	else if (key == "PAUSE")
	{
		bool flag = true;
		while (flag)
		{
			if (_kbhit())
			{
				int key_code = _getch();
				switch (key_code)
				{
				case 27:// is End button
					quick_exit(0);
				case 80:// is P
				case 112:// is p
					flag = false;
					break;
				default:
					break;
				}
			}
		}
	} 


	for (auto ctr : _enemy_tanks)
	{
		key = ctr->RandAction(); 
		if (key == "UP")
		{
			ctr->SetDirection(key);
			int k = ctr->GetPosition().second;
			int m = ctr->GetPosition().first;

			if (objects.second[k - 1][m] == (*_fields.begin())->Figure())
			{
				k = ctr->GetPosition().second - 1;
				m = ctr->GetPosition().first;
				ctr->SetPosition(m, k);
				objects.second[k][m] = ctr->Figure();
				objects.second[k + 1][m] = (*_fields.begin())->Figure();
			}
			else
			{
				objects.second[k][m] = ctr->Figure();
			}
		}
		else if (key == "LEFT")
		{
			ctr->SetDirection(key);
			int k = ctr->GetPosition().second;
			int m = ctr->GetPosition().first;

			if (objects.second[k][m - 1] == (*_fields.begin())->Figure())
			{
				k = ctr->GetPosition().second;
				m = ctr->GetPosition().first - 1;
				ctr->SetPosition(m, k);
				objects.second[k][m] = ctr->Figure();
				objects.second[k][m + 1] = (*_fields.begin())->Figure();
			}
			else
				objects.second[k][m] = ctr->Figure();
		}
		else if (key == "DOWN")
		{
			ctr->SetDirection(key);
			int k = ctr->GetPosition().second;
			int m = ctr->GetPosition().first;

			if (objects.second[k + 1][m] == (*_fields.begin())->Figure())
			{
				k = ctr->GetPosition().second + 1;
				m = ctr->GetPosition().first;
				ctr->SetPosition(m, k);
				objects.second[k][m] = ctr->Figure();
				objects.second[k - 1][m] = (*_fields.begin())->Figure();
			}
			else
				objects.second[k][m] = ctr->Figure();
		}
		else if (key == "RIGHT")
		{
			ctr->SetDirection(key);
			int k = ctr->GetPosition().second;
			int m = ctr->GetPosition().first;

			if (objects.second[k][m + 1] == (*_fields.begin())->Figure())
			{
				k = ctr->GetPosition().second;
				m = ctr->GetPosition().first + 1;
				ctr->SetPosition(m, k);
				objects.second[k][m] = ctr->Figure();
				objects.second[k][m - 1] = (*_fields.begin())->Figure();
			}
			else
				objects.second[k][m] = ctr->Figure();
		}
		else if (key == "FIRE")
		{
			ctr->Fire();
		}
	}

	_bullets = Bullet::storage;

	if (_bullets.size() != NULL)
	{
		for (auto ctr : _bullets)
		{
			int dir = ctr->cur_dir;
			int k = ctr->GetPosition().second;
			int m = ctr->GetPosition().first;

			if (dir == 0 || dir == 1)
			{
				if(ctr->GetPosition() == ctr->parent_coord && !ctr->activate)
				//if (m == _MTank->GetPosition().first && k == _MTank->GetPosition().second)
					///когда танк выстрелил
				{
					ctr->activate = true;
					if (objects.second[k - 1][m] == (*_fields.begin())->Figure())
						///когда впереди пустое поле
					{
						k = ctr->GetPosition().second - 1;
						m = ctr->GetPosition().first;
						ctr->SetPosition(m, k);
						objects.second[k][m] = ctr->Figure();
					}
					else
					{
						bool match = false;
						for (auto a : _borders)
						{
							if (a->GetPosition() == std::make_pair(k - 1, m))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								break;
							}
						}
						if (!match)
						{
							for (auto a : _walls)
							{
								if (a->GetPosition() == std::make_pair(m, k - 1))
								{
									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
										objects.first[2].erase(it2);
										objects.second[k - 1][m] = (*_fields.begin())->Figure();
									}
									else
									{
										a->SetHP(a->GetHP() - 1);
										objects.second[k - 1][m] = a->Figure();
									}
									break;
								}
							}
							if (match) break;

							for (auto a : _tanks)
							{
								Tanks* t = nullptr;
								for (auto b : _tanks)
								{
									auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
									if (it1 != objects.first[3].end())
									{
										t = b;
										break;
									}
								}

								if (a->GetPosition() == std::make_pair(m, k - 1))
								{
									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
											objects.first[3].erase(it2);
										}
										else
										{
											if (ctr->GetParent() == t)
											{
												MainTank::IncreaseScore();
											}
											it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
											objects.first[5].erase(it2);
										}
										objects.second[k - 1][m] = (*_fields.begin())->Figure();
									}
									else
									{
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
										}
										
										a->SetHP(a->GetHP() - 1);
										objects.second[k - 1][m] = a->Figure();
									}

									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									break;
								}
							}
							if (match) break;

							for (auto a : _bullets)
							{
								if (a->GetPosition() == std::make_pair(m, k - 1))
								{
									match = true;
									ctr->Destroy();
									a->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);
									auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
									Bullet::storage.erase(it2);
									objects.second[k - 1][m] = (*_fields.begin())->Figure();
									break;
								}
							}
							if (match) break;
						}
					}
					
				}

 				else if (objects.second[k - 1][m] == (*_fields.begin())->Figure())
						///остальной путь
						///когда впереди пустое поле
				{
					k = ctr->GetPosition().second - 1;
					m = ctr->GetPosition().first;
					ctr->SetPosition(m, k);
					objects.second[k][m] = ctr->Figure();
					objects.second[k + 1][m] = (*_fields.begin())->Figure();
				}
				else
				{
					bool match = false;
					for (auto a : _borders)
					{
						if (a->GetPosition() == std::make_pair(k - 1, m))
						{
							match = true;
							ctr->Destroy();
							auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
							Bullet::storage.erase(it1);
							objects.second[k][m] = (*_fields.begin())->Figure();
							break;
						}
					}
					if (!match)
					{
						for (auto a : _walls)
						{
							if (a->GetPosition() == std::make_pair(m, k - 1))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);

								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
									objects.first[2].erase(it2);
									objects.second[k - 1][m] = (*_fields.begin())->Figure();
								}
								else
								{
									a->SetHP(a->GetHP() - 1);
									objects.second[k - 1][m] = a->Figure();
								}
								objects.second[k][m] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;

						for (auto a : _tanks)
						{
							Tanks* t = nullptr;
							for (auto b : _tanks)
							{
								auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
								if (it1 != objects.first[3].end())
								{
									t = b;
									break;
								}
							}
							if (a->GetPosition() == std::make_pair(m, k - 1))
							{
								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
										objects.first[3].erase(it2);
									}
									else
									{
										if (ctr->GetParent() == t)
										{
											MainTank::IncreaseScore();
										}
										it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
										objects.first[5].erase(it2);
									}
									objects.second[k - 1][m] = (*_fields.begin())->Figure();
								}
								else
								{
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
									}

									a->SetHP(a->GetHP() - 1);
									objects.second[k - 1][m] = a->Figure();
								}

								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								objects.second[k][m] = (*_fields.begin())->Figure();

								break;
							}
						}
						if (match) break;

						for (auto a : _bullets)
						{
							if (a->GetPosition() == std::make_pair(m, k - 1))
							{
								match = true;
								ctr->Destroy();
								a->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
								Bullet::storage.erase(it2);
								objects.second[k][m] = (*_fields.begin())->Figure();
								objects.second[k - 1][m] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;
					}
				}
			}
			else if(dir == 2)
			{
				if (ctr->GetPosition() == ctr->parent_coord && !ctr->activate)
					///когда танк выстрелил
				{
					ctr->activate = true;
					if (objects.second[k + 1][m] == (*_fields.begin())->Figure())
						///когда впереди пустое поле
					{
						k = ctr->GetPosition().second + 1;
						m = ctr->GetPosition().first;
						ctr->SetPosition(m, k);
						objects.second[k][m] = ctr->Figure();
					}
					else
					{
						bool match = false;
						for (auto a : _borders)
						{
							if (a->GetPosition() == std::make_pair(k + 1, m))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								break;
							}
						}
						if (!match)
						{
							for (auto a : _walls)
							{
								if (a->GetPosition() == std::make_pair(m, k + 1))
								{
									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
										objects.first[2].erase(it2);
										objects.second[k + 1][m] = (*_fields.begin())->Figure();
									}
									else
									{
										a->SetHP(a->GetHP() - 1);
										objects.second[k + 1][m] = a->Figure();
									}
									break;
								}
							}
							if (match) break;

							for (auto a : _tanks)
							{
								Tanks* t = nullptr;
								for (auto b : _tanks)
								{
									auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
									if (it1 != objects.first[3].end())
									{
										t = b;
										break;
									}
								}

								if (a->GetPosition() == std::make_pair(m, k + 1))
								{
									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
											objects.first[3].erase(it2);
										}
										else
										{
											if (ctr->GetParent() == t)
											{
												MainTank::IncreaseScore();
											}
											it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
											objects.first[5].erase(it2);
										}
										objects.second[k + 1][m] = (*_fields.begin())->Figure();
									}
									else
									{
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
										}

										a->SetHP(a->GetHP() - 1);
										objects.second[k + 1][m] = a->Figure();
									}

									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									break;
								}
							}
							if (match) break;

							for (auto a : _bullets)
							{
								if (a->GetPosition() == std::make_pair(m, k + 1))
								{
									match = true;
									ctr->Destroy();
									a->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);
									auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
									Bullet::storage.erase(it2);
									objects.second[k + 1][m] = (*_fields.begin())->Figure();
									break;
								}
							}
							if (match) break;
						}
					}

				}
				else if (objects.second[k + 1][m] == (*_fields.begin())->Figure())
					///остальной путь
					///когда впереди пустое поле
				{
					k = ctr->GetPosition().second + 1;
					m = ctr->GetPosition().first;
					ctr->SetPosition(m, k);
					objects.second[k][m] = ctr->Figure();
					objects.second[k - 1][m] = (*_fields.begin())->Figure();
				}
				else
				{
					bool match = false;
					for (auto a : _borders)
					{
						if (a->GetPosition() == std::make_pair(k + 1, m))
						{
							match = true;
							ctr->Destroy();
							auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
							Bullet::storage.erase(it1);
							objects.second[k][m] = (*_fields.begin())->Figure();
							break;
						}
					}
					if (!match)
					{
						for (auto a : _walls)
						{
							if (a->GetPosition() == std::make_pair(m, k + 1))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);

								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
									objects.first[2].erase(it2);
									objects.second[k + 1][m] = (*_fields.begin())->Figure();
								}
								else
								{
									a->SetHP(a->GetHP() - 1);
									objects.second[k + 1][m] = a->Figure();
								}
								objects.second[k][m] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;

						for (auto a : _tanks)
						{
							Tanks* t = nullptr;
							for (auto b : _tanks)
							{
								auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
								if (it1 != objects.first[3].end())
								{
									t = b;
									break;
								}
							}

							if (a->GetPosition() == std::make_pair(m, k + 1))
							{
								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
										objects.first[3].erase(it2);
									}
									else
									{
										if (ctr->GetParent() == t)
										{
											MainTank::IncreaseScore();
										}
										it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
										objects.first[5].erase(it2);
									}
									objects.second[k + 1][m] = (*_fields.begin())->Figure();
								}
								else
								{
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
									}

									a->SetHP(a->GetHP() - 1);
									objects.second[k + 1][m] = a->Figure();
								}
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								objects.second[k][m] = (*_fields.begin())->Figure();

								break;
							}
						}
						if (match) break;

						for (auto a : _bullets)
						{
							if (a->GetPosition() == std::make_pair(m, k + 1))
							{
								match = true;
								ctr->Destroy();
								a->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
								Bullet::storage.erase(it2);
								objects.second[k][m] = (*_fields.begin())->Figure();
								objects.second[k + 1][m] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;
					}
				}
			}
			else if (dir == 3)
			{
				if (ctr->GetPosition() == ctr->parent_coord && !ctr->activate)
					///когда танк выстрелил
				{
					ctr->activate = true;
					if (objects.second[k][m - 1] == (*_fields.begin())->Figure())
						///когда впереди пустое поле
					{
						k = ctr->GetPosition().second;
						m = ctr->GetPosition().first - 1;
						ctr->SetPosition(m, k);
						objects.second[k][m] = ctr->Figure();
					}
					else
					{
						bool match = false;
						for (auto a : _borders)
						{
							if (a->GetPosition() == std::make_pair(k, m - 1))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								break;
							}
						}
						if (!match)
						{
							for (auto a : _walls)
							{
								if (a->GetPosition() == std::make_pair(m - 1, k))
								{
									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
										objects.first[2].erase(it2);
										objects.second[k][m - 1] = (*_fields.begin())->Figure();
									}
									else
									{
										a->SetHP(a->GetHP() - 1);
										objects.second[k][m - 1] = a->Figure();
									}
									break;
								}
							}
							if (match) break;

							for (auto a : _tanks)
							{
								Tanks* t = nullptr;
								for (auto b : _tanks)
								{
									auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
									if (it1 != objects.first[3].end())
									{
										t = b;
										break;
									}
								}

								if (a->GetPosition() == std::make_pair(m - 1, k))
								{
									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
											objects.first[3].erase(it2);
										}
										else
										{
											if (ctr->GetParent() == t)
											{
												MainTank::IncreaseScore();
											}
											it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
											objects.first[5].erase(it2);
										}
										objects.second[k][m - 1] = (*_fields.begin())->Figure();
									}
									else
									{
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
										}

										a->SetHP(a->GetHP() - 1);
										objects.second[k][m - 1] = a->Figure();
									}
									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									break;
								}
							}
							if (match) break;

							for (auto a : _bullets)
							{
								if (a->GetPosition() == std::make_pair(m - 1, k))
								{
									match = true;
									ctr->Destroy();
									a->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);
									auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
									Bullet::storage.erase(it2);
									objects.second[k][m - 1] = (*_fields.begin())->Figure();
									break;
								}
							}
							if (match) break;
						}
					}

				}
				else if (objects.second[k][m - 1] == (*_fields.begin())->Figure())
					///остальной путь
					///когда впереди пустое поле
				{
					k = ctr->GetPosition().second;
					m = ctr->GetPosition().first - 1;
					ctr->SetPosition(m, k);
					objects.second[k][m] = ctr->Figure();
					objects.second[k][m + 1] = (*_fields.begin())->Figure();
				}
				else
				{
					bool match = false;
					for (auto a : _borders)
					{
						if (a->GetPosition() == std::make_pair(k, m - 1))
						{
							match = true;
							ctr->Destroy();
							auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
							Bullet::storage.erase(it1);
							objects.second[k][m] = (*_fields.begin())->Figure();
							break;
						}
					}
					if (!match)
					{
						for (auto a : _walls)
						{
							if (a->GetPosition() == std::make_pair(m - 1, k))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								
								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
									objects.first[2].erase(it2);
									objects.second[k][m - 1] = (*_fields.begin())->Figure();
								}
								else
								{
									a->SetHP(a->GetHP() - 1);
									objects.second[k][m - 1] = a->Figure();
								}
								objects.second[k][m] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;

						for (auto a : _tanks)
						{
							Tanks* t = nullptr;
							for (auto b : _tanks)
							{
								auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
								if (it1 != objects.first[3].end())
								{
									t = b;
									break;
								}
							}

							if (a->GetPosition() == std::make_pair(m - 1, k))
							{
								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
										objects.first[3].erase(it2);
									}
									else
									{
										if (ctr->GetParent() == t)
										{
											MainTank::IncreaseScore();
										}
										it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
										objects.first[5].erase(it2);
									}
									objects.second[k][m - 1] = (*_fields.begin())->Figure();
								}
								else
								{
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
									}

									a->SetHP(a->GetHP() - 1);
									objects.second[k][m - 1] = a->Figure();
								}
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								objects.second[k][m] = (*_fields.begin())->Figure();

								break;
							}
						}
						if (match) break;

						for (auto a : _bullets)
						{
							if (a->GetPosition() == std::make_pair(m - 1, k))
							{
								match = true;
								ctr->Destroy();
								a->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
								Bullet::storage.erase(it2);
								objects.second[k][m] = (*_fields.begin())->Figure();
								objects.second[k][m - 1] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;
					}
				}
			}
			else if (dir == 4)
			{
				if (ctr->GetPosition() == ctr->parent_coord && !ctr->activate)
					///когда танк выстрелил
				{
					ctr->activate = true;
					if (objects.second[k][m + 1] == (*_fields.begin())->Figure())
						///когда впереди пустое поле
					{
						k = ctr->GetPosition().second;
						m = ctr->GetPosition().first + 1;
						ctr->SetPosition(m, k);
						objects.second[k][m] = ctr->Figure();
					}
					else
					{
						bool match = false;
						for (auto a : _borders)
						{
							if (a->GetPosition() == std::make_pair(k, m + 1))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								break;
							}
						}
						if (!match)
						{
							for (auto a : _walls)
							{
								if (a->GetPosition() == std::make_pair(m + 1, k))
								{
									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
										objects.first[2].erase(it2);
										objects.second[k][m + 1] = (*_fields.begin())->Figure();
									}
									else
									{
										a->SetHP(a->GetHP() - 1);
										objects.second[k][m + 1] = a->Figure();
									}
									break;
								}
							}
							if (match) break;

							for (auto a : _tanks)
							{
								Tanks* t = nullptr;
								for (auto b : _tanks)
								{
									auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
									if (it1 != objects.first[3].end())
									{
										t = b;
										break;
									}
								}

								if (a->GetPosition() == std::make_pair(m + 1, k))
								{
									if (a->GetHP() == 1)
									{
										a->Destroy();
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
											objects.first[3].erase(it2);
										}
										else
										{
											if (ctr->GetParent() == t)
											{
												MainTank::IncreaseScore();
											}
											it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
											objects.first[5].erase(it2);
										}
										objects.second[k][m + 1] = (*_fields.begin())->Figure();
									}
									else
									{
										auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
										if (it2 != objects.first[3].end())
										{
											MainTank::SetHealth(a->GetHP() - 1);
										}

										a->SetHP(a->GetHP() - 1);
										objects.second[k][m + 1] = a->Figure();
									}
									match = true;
									ctr->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);

									break;
								}
							}
							if (match) break;

							for (auto a : _bullets)
							{
								if (a->GetPosition() == std::make_pair(m + 1, k))
								{
									match = true;
									ctr->Destroy();
									a->Destroy();
									auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
									Bullet::storage.erase(it1);
									auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
									Bullet::storage.erase(it2);
									objects.second[k][m + 1] = (*_fields.begin())->Figure();
									break;
								}
							}
							if (match) break;
						}
					}

				}

				else if (objects.second[k][m + 1] == (*_fields.begin())->Figure())
					///остальной путь
					///когда впереди пустое поле
				{
					k = ctr->GetPosition().second;
					m = ctr->GetPosition().first + 1;
					ctr->SetPosition(m, k);
					objects.second[k][m] = ctr->Figure();
					objects.second[k][m - 1] = (*_fields.begin())->Figure();
				}
				else
				{
					bool match = false;
					for (auto a : _borders)
					{
						if (a->GetPosition() == std::make_pair(k, m + 1))
						{
							match = true;
							ctr->Destroy();
							auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
							Bullet::storage.erase(it1);
							objects.second[k][m] = (*_fields.begin())->Figure();
							break;
						}
					}
					if (!match)
					{
						for (auto a : _walls)
						{
							if (a->GetPosition() == std::make_pair(m + 1, k))
							{
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);

								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[2].begin(), objects.first[2].end(), a);
									objects.first[2].erase(it2);
									objects.second[k][m + 1] = (*_fields.begin())->Figure();
								}
								else
								{
									a->SetHP(a->GetHP() - 1);
									objects.second[k][m + 1] = a->Figure();
								}
								objects.second[k][m] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;

						for (auto a : _tanks)
						{
							Tanks* t = nullptr;
							for (auto b : _tanks)
							{
								auto it1 = std::find(objects.first[3].begin(), objects.first[3].end(), b);
								if (it1 != objects.first[3].end())
								{
									t = b;
									break;
								}
							}

							if (a->GetPosition() == std::make_pair(m + 1, k))
							{
								if (a->GetHP() == 1)
								{
									a->Destroy();
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
										objects.first[3].erase(it2);
									}
									else
									{
										if (ctr->GetParent() == t)
										{
											MainTank::IncreaseScore();
										}
										it2 = std::find(objects.first[5].begin(), objects.first[5].end(), a);
										objects.first[5].erase(it2);
									}
									objects.second[k][m + 1] = (*_fields.begin())->Figure();
								}
								else
								{
									auto it2 = std::find(objects.first[3].begin(), objects.first[3].end(), a);
									if (it2 != objects.first[3].end())
									{
										MainTank::SetHealth(a->GetHP() - 1);
									}

									a->SetHP(a->GetHP() - 1);
									objects.second[k][m + 1] = a->Figure();
								}
								objects.second[k][m] = (*_fields.begin())->Figure();
								match = true;
								ctr->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);

								break;
							}
						}
						if (match) break;

						for (auto a : _bullets)
						{
							if (a->GetPosition() == std::make_pair(m + 1, k))
							{
								match = true;
								ctr->Destroy();
								a->Destroy();
								auto it1 = std::find(Bullet::storage.begin(), Bullet::storage.end(), ctr);
								Bullet::storage.erase(it1);
								auto it2 = std::find(Bullet::storage.begin(), Bullet::storage.end(), a);
								Bullet::storage.erase(it2);
								objects.second[k][m] = (*_fields.begin())->Figure();
								objects.second[k][m + 1] = (*_fields.begin())->Figure();
								break;
							}
						}
						if (match) break;
					}
				}
			}
		}
	}
}

void setcur(int x, int y)//установка курсора на позицию  x y 
{
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
};

void Draw(vector<vector<pair<char, int>>> objects)
{
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	setcur(0, 0);
	for (int i = 0; i < (int)objects.size(); i++)
	{
		for (int j = 0; j < (int)objects[i].size(); j++)
		{
			SetConsoleTextAttribute(hConsole, objects[i][j].second);
			cout << objects[i][j].first;
		}
		cout << endl;
	}
	SetConsoleTextAttribute(hConsole, 15);
	cout << "Lives: ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << MainTank::GetHealth();
	SetConsoleTextAttribute(hConsole, 15);
	cout << "\t\t\tScore: ";
	SetConsoleTextAttribute(hConsole, 12);
	cout << MainTank::GetScore();
	setcur(0, 0);
}

int main()
{
	Menu();
	pair<vector<vector<Objects*>>, vector<vector<pair<char, int>>>> match = Initialize();

	while(true)
	{
		string key = Input();
		Logic(match, key);
		Draw(match.second);
	}

	return 0;
}