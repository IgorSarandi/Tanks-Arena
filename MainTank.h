#pragma once
#include "Tanks.h"
class MainTank :
	public Tanks
	///Singleton
{
public:
	MainTank(const MainTank&) = delete;

	MainTank& operator=(MainTank&) = delete;

	~MainTank();

	virtual std::pair<char, int> Figure() override;

	static MainTank& getSingleton();

	static void IncreaseScore();

	static int GetScore();

	static void SetHealth(int);

	static std::string GetHealth();

private:
	MainTank() {}
	static int health;
	static int score;
};

