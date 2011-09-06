#ifndef _RANDGEN_H_
#define _RANDGEN_H_

class RandGen
{
public:
	static void SetRandomSeed();
	static void SetSeed(unsigned int seed);

	static float GenerateFloat();
	static double GenerateDouble();
	static int GenerateInt();

	static float GenerateFloat(float min, float max);
	static double GenerateDouble(double min, double max);
	static int GenerateInt(int min, int max);

	static bool FlipCoin(float probability_true);
};

#endif