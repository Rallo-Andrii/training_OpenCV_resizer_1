#pragma once

#include "resizing.h"

void f(cv::Mat& image_in, cv::Mat& image_out)
{
	double per_x = 0.05f;
	double per_y = 0.05f;
	cv::Size size_in = image_in.size();
	cv::Size size_out(round(size_in.width * per_x), round(size_in.height * per_y));
	int type = image_in.type();

	cv::Mat image_out1(size_out, type);

	cv::Mat image_out_rect(size_out, type);
	//cv::Rect rect(0, size_in.height / 2, size_in.width, size_in.height/2);
	cv::Mat image_test_rect_in_1 = image_in(cv::Rect(0, 0, round(size_in.width / 2), round(size_in.height / 2)));
	cv::Mat image_test_rect_in_2 = image_in(cv::Rect(0, round(size_in.height / 2), round(size_in.width / 2), round(size_in.height / 2)));
	cv::Mat image_test_rect_in_3 = image_in(cv::Rect(round(size_in.width / 2), 0, round(size_in.width / 2), round(size_in.height / 2)));
	cv::Mat image_test_rect_in_4 = image_in(cv::Rect(round(size_in.width / 2), round(size_in.height / 2), round(size_in.width / 2), round(size_in.height / 2)));
	cv::Mat image_test_rect_out_1 = image_out_rect(cv::Rect(0, 0, round(size_out.width / 2), round(size_out.height / 2)));
	cv::Mat image_test_rect_out_2 = image_out_rect(cv::Rect(0, round(size_out.height / 2), round(size_out.width / 2), round(size_out.height / 2)));
	cv::Mat image_test_rect_out_3 = image_out_rect(cv::Rect(round(size_out.width / 2), 0, round(size_out.width / 2), round(size_out.height / 2)));
	cv::Mat image_test_rect_out_4 = image_out_rect(cv::Rect(round(size_out.width / 2), round(size_out.height / 2), round(size_out.width / 2), round(size_out.height / 2)));
	cv::resize(image_test_rect_in_1, image_test_rect_out_1, image_test_rect_out_1.size(), 0, 0, cv::INTER_LINEAR);
	cv::resize(image_test_rect_in_2, image_test_rect_out_2, image_test_rect_out_2.size(), 0, 0, cv::INTER_LINEAR);
	cv::resize(image_test_rect_in_3, image_test_rect_out_3, image_test_rect_out_3.size(), 0, 0, cv::INTER_LINEAR);
	cv::resize(image_test_rect_in_4, image_test_rect_out_4, image_test_rect_out_4.size(), 0, 0, cv::INTER_LINEAR);

	imwrite("Test_out_rect.jpg", image_out_rect);

	cv::Mat image_diff_main_and_rect(size_out, type);
	int count = compare(image_out1, image_out_rect, image_diff_main_and_rect);
	imwrite("Test_diff_main_and_rect.jpg", image_diff_main_and_rect);

	/*
	cv::Mat image_diff_main_and_out2(size_out, type);
	count = compare(image_out1, image_out2, image_diff_main_and_out2);
	imwrite("Test_diff_main_and_out2.jpg", image_diff_main_and_out2);

	cv::Mat image_diff_rect_and_out2(size_out, type);
	count = compare(image_out_rect, image_out2, image_diff_rect_and_out2);
	imwrite("Test_diff_rect_and_out2.jpg", image_diff_rect_and_out2);
	*/
}