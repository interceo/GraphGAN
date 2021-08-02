#pragma once
#include "Tensor.h"
#include <filesystem>

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace fs = std::filesystem;

class image
{
	std::vector<std::string> path;
public:
	Tensor GetImage(std::string path);
	void load_image(std::string dir);
	void list_path();
};
