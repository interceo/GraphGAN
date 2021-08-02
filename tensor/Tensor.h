#pragma once
#include <vector>
#include <iostream>

typedef std::vector<unsigned char> uvec;
typedef std::vector<float> fvec;
typedef std::vector<int> ivec;

struct TensorSize
{
	int depth; // �������
	int height; // ������
	int width; // ������
};

class Tensor
{
	fvec weight;
	unsigned int dw; // ������

public:
	TensorSize size;

	Tensor(int depth, int height, int width);
	Tensor(TensorSize s);
	Tensor();
	TensorSize GetSize();

	float& operator()(int d, int h, int w);         // ����������
	float operator()(int d, int h, int w) const;
	void operator()();
};