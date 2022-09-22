#include "Lin.h"

Lin::Lin(int num, ...) : num(num) {
	data.resize(num - 1);
	layers.resize(num);
	va_list factor;
	va_start(factor, num);
	for (int i = 0; i < num; i++)
	{
		layers[i].resize(va_arg(factor, int) + 1);
	}
	for (int i = 1; i < num; i++)
	{
		data[i - 1].resize(layers[i - 1].size() * layers[i].size());
	}
	va_end(factor);
	rand_init();
}

std::vector<float>& Lin::calc(std::vector<float>& start) {
	if (start.size() + 1 != layers[0].size())
	{
		throw std::exception("Invalid start layer size!\n");
	}

	for (int i = 0; i < start.size(); i++) {
		layers[0][i] = start[i];
	}

	for (int i = 0; i < num - 1; i++)
	{
		for (int k = 0; k < layers[i + 1].size() - 1; k++)
		{
			VLAST(layers[i]) = 1;
			for (int j = 0; j < layers[i].size(); j++)
			{
				layers[i + 1][k] += layers[i][j] * data[i][layers[i].size() * k + j];
			}
			this->exponential(layers[i + 1][k]);
		}
	}
	return VLAST(layers);
}

void Lin::learn(std::vector<float>& expected)
{
	if (expected.size() != VLAST(layers).size())
	{
		throw std::exception("Invalid end layer size!\n");
	}

	for (int i = 1; i < num; i++)
	{

	}
}

void Lin::rand_init(uint64_t seed) {
	for (auto& layer : data) {
		for (auto& num : layer) num = (float)random() / random.max();
	}
}

float Lin::exponential(float& f) {
	return f = 1.0 / (1.0 + exp(-f));
}