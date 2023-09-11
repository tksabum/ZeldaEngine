#pragma once

class Guilty
{
public:
	static Guilty& GetInstance();

public:
	int bossPhase = 1;

private:
	Guilty();
	~Guilty();
	Guilty(const Guilty& ref) = delete;
	Guilty& operator=(const Guilty& ref) = delete;
};

