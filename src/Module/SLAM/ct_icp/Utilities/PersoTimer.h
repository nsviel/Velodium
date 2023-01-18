#pragma once

#include <iostream>
#include <string>
#include <time.h>
#include <ctime>


using namespace std;

class PersoTimer
{
public:
	PersoTimer();
	~PersoTimer();

	void   tic();
	double toc(bool print=true);
	double toc(string str, bool print=true);

private:
	clock_t _t;
	double  _dt;
};