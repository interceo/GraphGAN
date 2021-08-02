#include "Tensor.h"

Tensor::Tensor(int depth, int height, int width)
{
	size.depth = depth;
	size.height = height;
	size.width = width;

	dw = depth * width;
	weight = fvec(dw * height, 0);
}

Tensor::Tensor(TensorSize s)
{
	size.depth = s.depth;
	size.height = s.height;
	size.width = s.width;

	dw = s.depth * s.width;
	weight = fvec(dw * s.height, 0);
}

Tensor::Tensor()
{
}

TensorSize Tensor::GetSize()
{
	std::cout << size.depth << "\t" << size.height << "\t" << size.width << "\n";
	return size;
}

float& Tensor::operator()(int d, int h, int w)
{
	return weight[h * dw + w * size.depth + d];
}

float Tensor::operator()(int d, int h, int w) const
{
	return weight[h * dw + w * size.depth + d];
}

void Tensor::operator()()
{
	for (int d = 0; d < size.depth; d++) {
		for (int i = 0; i < size.height; i++) {
			for (int j = 0; j < size.width; j++)
 				std::cout << weight[i * dw + j * size.depth + d] << " ";

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}
