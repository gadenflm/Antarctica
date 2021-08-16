#include <iostream>
#include <opencv2\opencv.hpp>

int main() {
	cv::Mat src = cv::imread("images\\src.bmp");
	
	cv::Mat gray_src;
	cv::cvtColor(src,gray_src,6);

	cv::imshow("Src",src);
	cv::imshow("Gray",gray_src);

	cv::waitKey(0);
	return 0;
}