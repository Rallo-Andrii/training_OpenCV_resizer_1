#pragma once

#include <opencv2\core.hpp>
#include <opencv2\imgcodecs.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>

#include <vector>
#include <string>

using namespace std;


void resize(cv::Mat& mat_in, cv::Mat& mat_out)
{
	cv::resize(mat_in, mat_out, mat_out.size(), 0, 0, cv::INTER_LINEAR);
}


struct images_conformity
{
	cv::Mat in;
	cv::Mat out;
};

vector<images_conformity> divide_to_parts(cv::Mat& image_in, cv::Mat& image_out, int rows, int cols)
{
	vector<images_conformity> parts(rows*cols);
	int counter = 0;
	for (int r = 0; r < rows; r++)
	{
		double div_in_r = (double)image_in.rows / rows;
		double div_out_r = (double)image_out.rows / rows;
		cv::Mat temp_in = image_in.rowRange(round(r * div_in_r), round((r + 1) * div_in_r));
		cv::Mat temp_out = image_out.rowRange(round(r * div_out_r), round((r + 1) * div_out_r));
		for (int c = 0; c < cols; c++)
		{
			double div_in_c = (double)temp_in.cols / cols;
			double div_out_c = (double)temp_out.cols / cols;
			parts[counter].in = temp_in.colRange(round(c * div_in_c), round((c + 1) * div_in_c));
			parts[counter].out = temp_out.colRange(round(c * div_out_c), round((c + 1) * div_out_c));
			counter++;
		}
	}
	return parts;
}

class vector_scheme
{
public:
	
	struct scheme
	{
		string name;
		std::vector<images_conformity> conf;
	};

	vector_scheme(cv::Mat& mat_in, cv::Mat& mat_out)
	{
		scheme sch;

		for (int i = 2; i <= 12; i++)
		{
			sch.name = "rows 1, cols " + to_string(i);
			sch.conf = divide_to_parts(mat_in, mat_out, 1, i);
			schs.push_back(sch);

			sch.name = "rows "+ to_string(i) + ", cols 1";
			sch.conf = divide_to_parts(mat_in, mat_out, i, 1);
			schs.push_back(sch);
		}

		for (int i = 2; i <= 6; i++)
		{
			sch.name = "rows 2, cols " + to_string(i);
			sch.conf = divide_to_parts(mat_in, mat_out, 2, i);
			schs.push_back(sch);

			sch.name = "rows " + to_string(i) + ", cols 2";
			sch.conf = divide_to_parts(mat_in, mat_out, i, 2);
			schs.push_back(sch);
		}

		for (int i = 3; i <= 4; i++)
		{
			sch.name = "rows 3, cols " + to_string(i);
			sch.conf = divide_to_parts(mat_in, mat_out, 3, i);
			schs.push_back(sch);

			sch.name = "rows " + to_string(i) + ", cols 3";
			sch.conf = divide_to_parts(mat_in, mat_out, i, 3);
			schs.push_back(sch);
		}
	}
	size_t size()
	{
		return schs.size();
	}
	scheme& operator[](int index)
	{
		return schs[index];
	}
private:
	vector<scheme> schs;
};



int compare(cv::Mat& mat_sample1, cv::Mat& mat_sample2, cv::Mat& difference)
{
	if (mat_sample1.size() != mat_sample2.size() || mat_sample1.size() != difference.size())
		throw std::invalid_argument("size of mat_sample1, mat_sample2 and difference must be equil");
	int count = 0;
	cv::Vec3b vec_equil(0,0,0);
	cv::Vec3b vec_notequil(255,255,255);
	for (int i = 0; i < mat_sample1.rows; i++)
	{
		for (int j = 0; j < mat_sample1.cols; j++)
		{
			if (mat_sample1.at<cv::Vec3b>(i, j) != mat_sample2.at<cv::Vec3b>(i, j))
			{
				difference.at<cv::Vec3b>(i, j) = vec_notequil;
				count++;
			}
			else
				difference.at<cv::Vec3b>(i, j) = vec_equil;
		}
	}
	return count;
}


int compare(cv::Mat& mat_sample1, cv::Mat& mat_sample2)
{
	if (mat_sample1.size() != mat_sample2.size())
		throw std::invalid_argument("size of mat_sample1 and mat_sample2");
	int count = 0;
	cv::Vec3b vec_equil(0, 0, 0);
	cv::Vec3b vec_notequil(255, 255, 255);
	for (int i = 0; i < mat_sample1.rows; i++)
	{
		for (int j = 0; j < mat_sample1.cols; j++)
		{
			if (mat_sample1.at<cv::Vec3b>(i, j) != mat_sample2.at<cv::Vec3b>(i, j))
				count++;
		}
	}
	return count;
}
