#include "ConvLayer.h"
#include "image.h"


int main()
{
	image load;
	//load.load_image("F:\\Foton\\!programdata\\גפופ\\test");
	//load.list_path();

	ConvLayer layer1(load.GetImage("F:\\Foton\\1.png"), 3, 3, 0, 3);
	//ConvLayer layer2(layer1.Forward(), 3, 3, 0, 1);
	Tensor o = layer1.Forward();
	layer1.Backward(o);

	cv::Mat im(o.size.height, o.size.width, CV_8UC3);

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < im.rows; h++) {
			for (int w = 0; w < im.cols; w++) {
				if (o(i, h, w) < 0)
					im.at<cv::Vec3b>(h, w)[i] = 0;
				else if (o(i, h, w) > 255)
					im.at<cv::Vec3b>(h, w)[i] = 255;
				else
					im.at<cv::Vec3b>(h, w)[i] = o(i, h, w);
			}
		}
	}

	cv::imshow("output", im);
	cv::waitKey(100000);
}