#pragma once
#include <iostream>
#include <stdarg.h>
#include <vector>
#include <random>

#define VLAST(X) (X[X.size() - 1])

class Lin {
	int num;
	std::random_device random;
	std::vector<std::vector<float>> data, layers;

public:
	Lin(int num, ...);
	~Lin() { }

	std::vector<float>& calc(std::vector<float>& start);
	void learn(std::vector<float>& expected);
	void rand_init(uint64_t seed = 0);

private:
	float exponential(float& f);
};