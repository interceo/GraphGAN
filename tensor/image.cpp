#include "image.h"

Tensor image::GetImage(std::string path)
{
	cv::Mat image = cv::imread(path);
	cv::imshow("input", image);
	cv::waitKey(1);
	Tensor image_tensor(3, image.rows, image.cols);

	for (int i = 0; i < 3; i++) {
		for (int h = 0; h < image.rows; h++) {
			for (int w = 0; w < image.cols; w++) {
				image_tensor(i, h, w) = (int)image.at<cv::Vec3b>(h, w)[i];
			}
		}
	}
	return image_tensor;
}

void image::load_image(std::string dir)
{
	for (fs::path p : fs::directory_iterator(dir)) {
		path.push_back(p.string());
		//std::reverse(d.begin(), d.end());
		//data.push_back(d);
	}
}

void image::list_path()
{
	for (long i = 0; i < path.size(); i++)
		std::cout << path[i] << "\n";
}
