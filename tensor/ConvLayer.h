#include "Tensor.h"
#include <random>

class ConvLayer
{
	Tensor input;
	Tensor output;

	std::vector<float> bias; // ��������

	std::vector<Tensor> del_filter; // ��������� ��������
	std::vector<float> del_bias; // ��������� ��������

	int filter_amount;
	int filter_size;
	int filter_deep;

	int add_null;         // ���-�� �������� � ���� �����������
	int conv_step;

public:
	std::vector<Tensor> filter; // �������
	ConvLayer(Tensor input, int filter_amount, int filter_size, int add_null, int conv_step);
	Tensor Forward();
	Tensor Backward(const Tensor& dout);
	void UpdateWeight();
private:
	void init_weight();
};

