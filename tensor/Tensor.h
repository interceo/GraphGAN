#pragma once
#include <vector>
#include <iostream>

typedef std::vector<unsigned char> uvec;
typedef std::vector<float> fvec;
typedef std::vector<int> ivec;

struct TensorSize
{
	int depth; // глубина
	int height; // высота
	int width; // ширина
};

class Tensor
{
	fvec weight;
	unsigned int dw; // индекс

public:
	TensorSize size;

	Tensor(int depth, int height, int width);
	Tensor(TensorSize s);
	Tensor();
	TensorSize GetSize();

	float& operator()(int d, int h, int w);         // присвоение
	float operator()(int d, int h, int w) const;
	void operator()();
};