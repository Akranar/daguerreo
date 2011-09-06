#include "Core_RandGen.h"

#include <cstdlib>
#include <time.h>

void RandGen::SetRandomSeed()
{
	time_t seconds;
	time(&seconds);
	unsigned int seed = (unsigned int) seconds;
	std::srand(seed);
}
void RandGen::SetSeed(unsigned int seed)
{
	std::srand(seed);
}

float RandGen::GenerateFloat()
{
	return ((float) std::rand()) / RAND_MAX;
}
double RandGen::GenerateDouble()
{
	return ((double) std::rand()) / RAND_MAX;
}
int RandGen::GenerateInt()
{
	return std::rand();
}

float RandGen::GenerateFloat(float min, float max)
{
	return GenerateFloat() * (max-min) + min;
}
double RandGen::GenerateDouble(double min, double max)
{
	return GenerateDouble() * (max-min) + min;
}
int RandGen::GenerateInt(int min, int max)
{
	return (std::rand() % (max-min)) + min;
}

bool RandGen::FlipCoin(float probability_true)
{
	return GenerateFloat() < probability_true;
}