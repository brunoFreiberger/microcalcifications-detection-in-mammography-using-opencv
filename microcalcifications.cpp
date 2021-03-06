#include "stdafx.h"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

//Bruno Henrique Freiberger.

using namespace cv;

Mat img, img_dilated, tophat_open, tophat_close, img_sum, img_subtract, img_clean, img_final;

int const threshold_type = 3; // threshold ( 3 = binarization )
int const threshold_value = 20;
int const threshold_max_value = 255;
int const size = 4;

cv::String const img_input_path = "C:/Users/Bruno Henrique/Desktop/mammography.png";
cv::String const img_output_path = "C:/Users/Bruno Henrique/Desktop/final.png";

int main(int, char** argv)
{
	img = imread(img_input_path, IMREAD_COLOR); //Read image

												//Defines the structuring element - step 1
	Mat structuringElement = getStructuringElement(cv::MORPH_ELLIPSE,
		cv::Size(2 * size + 1, 2 * size + 1),
		cv::Point(size, size));

	//Dilate the image - step 2
	dilate(img, img_dilated, structuringElement);

	//Tophat - step 3
	morphologyEx(img, tophat_open, cv::MORPH_TOPHAT, structuringElement);

	//Blackhat - step 4
	morphologyEx(img, tophat_close, cv::MORPH_BLACKHAT, structuringElement);

	//Sum original image with the open tophat - step 5
	cv::add(img, tophat_open, img_sum);

	//Subtract original image with the close tophat - step 6
	cv::subtract(img_sum, tophat_close, img_subtract);

	//Remove the background image - step 7 
	cv::subtract(img_subtract, img_dilated, img_clean);

	//Binarize - step 8
	threshold(img_clean, img_final, threshold_value, threshold_max_value, threshold_type);

	//Show images
	imshow("Original image", img);
	imshow("Final image", img_final);

	//Save image in directory
	imwrite(img_output_path, img_final);

	waitKey(0);

	return 0;
}


