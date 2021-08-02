#include "ConvLayer.h"

ConvLayer::ConvLayer(Tensor input, int filter_amount, int filter_size, int add_null, int conv_step)
{
	output.size.width = (input.size.width - filter_amount + 2 * add_null) / conv_step + 1;
	output.size.height = (input.size.height - filter_amount + 2 * add_null) / conv_step + 1;
	output.size.depth = filter_amount;
	Tensor out(output.size.depth, output.size.height, output.size.width);
	output = out;

	this->add_null = add_null;
	this->conv_step = conv_step;

	this->filter_amount = filter_amount;
	this->filter_size = filter_size;
	this->filter_deep = input.size.depth;

	this->input = input;

	filter = std::vector<Tensor>(filter_amount, Tensor(input.size.depth, filter_size, filter_size));
	del_filter = std::vector<Tensor>(filter_amount, Tensor(input.size.depth, filter_size, filter_size));

	bias = std::vector<float>(filter_amount, 0.05f);
	del_bias = std::vector<float>(filter_amount);
	init_weight();
}

Tensor ConvLayer::Forward()
{
	for (int i = 0; i < output.size.depth; i++) {
		for (int h = 0; h < output.size.height; h++) {
			for (int w = 0; w < output.size.width; w++) {
				float sum = bias[i];

				for (int hf = 0; hf < filter_size; hf++) {
					for (int wf = 0; wf < filter_size; wf++) {
						int i0 = conv_step * h + hf - add_null;
						int j0 = conv_step * w + wf - add_null;

						if (i0 < 0 || i0 >= input.size.height || j0 < 0 || j0 >= input.size.width)
							continue;

						for (int c = 0; c < filter_deep; c++)
							sum += input(c, i0, j0) * filter[i](c, hf, wf);
					}
				}
				//output();
				output(i, h, w) = sum;
			}
		}
	}
	return output;
}

Tensor ConvLayer::Backward(const Tensor& dout)
{
	TensorSize del_size; // размер дельт

	// расчитываем размер для дельт
	del_size.height = conv_step * (output.size.height - 1) + 1;
	del_size.width = conv_step * (output.size.width - 1) + 1;
	del_size.depth = output.size.depth;

	Tensor deltas(del_size); // создаём тензор для дельт

	// расчитываем значения дельт
	for (int d = 0; d < del_size.depth; d++)
		for (int i = 0; i < output.size.height; i++)
			for (int j = 0; j < output.size.width; j++)
				deltas(d, i * conv_step, j * conv_step) = dout(d, i, j);

	// расчитываем градиенты весов фильтров и смещений
	for (int f = 0; f < filter_amount; f++) {
		for (int y = 0; y < del_size.height; y++) {
			for (int x = 0; x < del_size.width; x++) {
				double delta = deltas(f, y, x); // запоминаем значение градиента

				for (int i = 0; i < filter_size; i++) {
					for (int j = 0; j < filter_size; j++) {
						int i0 = i + y - add_null;
						int j0 = j + x - conv_step;

						// игнорируем выходящие за границы элементы
						if (i0 < 0 || i0 >= input.size.height || j0 < 0 || j0 >= input.size.width)
							continue;

						// наращиваем градиент фильтра
						for (int c = 0; c < filter_deep; c++)
							del_filter[f](c, i, j) += delta * input(c, i0, j0);
					}
				}

				del_bias[f] += delta; // наращиваем градиент смещения
			}
		}
	}

	int pad = filter_size - add_null - 1; // заменяем величину дополнения
	Tensor dX(input.size); // создаём тензор градиентов по входу

	// расчитываем значения градиента
	for (int y = 0; y < input.size.height; y++) {
		for (int x = 0; x < input.size.width; x++) {
			for (int c = 0; c < filter_deep; c++) {
				double sum = 0; // сумма для градиента

				// идём по всем весовым коэффициентам фильтров
				for (int i = 0; i < filter_size; i++) {
					for (int j = 0; j < filter_size; j++) {
						int i0 = y + i - pad;
						int j0 = x + j - pad;

						// игнорируем выходящие за границы элементы
						if (i0 < 0 || i0 >= del_size.height || j0 < 0 || j0 >= del_size.width)
							continue;

						// суммируем по всем фильтрам
						for (int f = 0; f < filter_amount; f++)
							sum += filter[f](c, filter_size - i - 1, filter_size - j - 1) * deltas(f, i0, j0); // добавляем произведение повёрнутых фильтров на дельты
					}
				}

				dX(c, y, x) = sum; // записываем результат в тензор градиента
			}
		}
	}

	return dX; // возвращаем тензор градиентов
}

void ConvLayer::UpdateWeight()
{

}

void ConvLayer::init_weight()
{
	std::default_random_engine generator;
	std::normal_distribution<float> distribution(0.0, sqrt(2.0 / (filter_size * filter_size * input.size.depth)));

	for (int i = 0; i < filter_amount; i++) {
		filter[i](i, 0, 0) = -1;
		filter[i](i, 0, 1) = -2;
		filter[i](i, 0, 2) = -1;

		filter[i](i, 1, 0) = 0;
		filter[i](i, 1, 1) = 0;
		filter[i](i, 1, 2) = 0;

		filter[i](i, 2, 0) = 1;
		filter[i](i, 2, 1) = 2;
		filter[i](i, 2, 2) = 1;
	}
	return;

	for (int i = 0; i < filter_amount; i++) {
		for (int h = 0; h < filter_size; h++) {
			for (int w = 0; w < filter_size; w++) {
				filter[i](i, h, w) = distribution(generator);
			}
		}
	}
}