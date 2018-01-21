#include <iostream>
//#include <ios>
#include <iomanip>
#include <chrono>
//#include <vector>
//#include <opencv2\core.hpp>
//#include <opencv2\imgcodecs.hpp>
//#include <opencv2\highgui.hpp>
//#include <opencv2\imgproc.hpp>
#include <thread>
#include <future>

#include "resizing.h"

using namespace std;


int main()
{
	cout.sync_with_stdio(false);
	int width = 600;
	int height = 480;
	//double per_x = 0.2f;
	//double per_y = 0.2f;
	cv::Mat image_in;
	cv::Size size_in;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	
	start = chrono::system_clock::now();
	image_in = cv::imread("Test_in.jpg", cv::IMREAD_COLOR  /*& cv::IMREAD_LOAD_GDAL*/);
	end = chrono::system_clock::now();
	int64 microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

	if (image_in.empty()) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		return -1;
	}
	cout << "imread == " << setw(9) << microseconds << endl;

	size_in = image_in.size();
	cv::Size size_out(width, height);
	int type = image_in.type();


	cv::Mat image_out_main(size_out, type);
	start = chrono::system_clock::now();
	cv::resize(image_in, image_out_main, image_out_main.size(), 0, 0, cv::INTER_LINEAR);
	end = chrono::system_clock::now();
	microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	cout << "resize full image in one main thread == " << setw(9) << microseconds << endl;
	imwrite("Test_out_main.jpg", image_out_main);

	cv::Mat image_out2(size_out, type);
	cv::Mat difference(size_out, type);

	vector_scheme VS(image_in, image_out2);

	for (int sc = 0; sc < VS.size(); sc++)
	{
		cout << "Scheme: " << VS[sc].name;
		size_t size = VS[sc].conf.size();
		std::vector<thread> thrd(size-1);
		start = chrono::system_clock::now();
		for (size_t i = 1; i < size; i++)
			thrd[i-1] = thread(resize, ref(VS[sc].conf[i].in), ref(VS[sc].conf[i].out));
		resize(VS[sc].conf[0].in, VS[sc].conf[0].out);
		for (size_t i = 1; i < size; i++)
			thrd[i-1].join();
		end = chrono::system_clock::now();
		microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		int diff = compare(image_out_main, image_out2);
		//int diff = compare(image_out_main, image_out2, difference);
		cout << ". Resized in " << size << " threads == " << setw(9) << microseconds;
		cout << ". Difference " << diff;
		//bool b = imwrite("Test_out_diff\\Test_out_" + VS[sc].name +  "_t.jpg", difference);
		//b = imwrite("Test_out\\Test_out_" + VS[sc].name + "_t.jpg", image_out2);

		cout << endl;
		cout << "Scheme: " << VS[sc].name;
		std::vector<std::future<void>> fut(size-1);
		start = chrono::system_clock::now();
		for (size_t i = 1; i < size; i++)
			fut[i-1] = std::async(std::launch::async, resize, ref(VS[sc].conf[i].in), ref(VS[sc].conf[i].out));
		resize(VS[sc].conf[0].in, VS[sc].conf[0].out);
		for (size_t i = 1; i < size; i++)
			fut[i - 1].wait();
		end = chrono::system_clock::now();
		microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
		diff = compare(image_out_main, image_out2, difference);
		cout << ". Resized in " << size << " async   == " << setw(9) << microseconds;
		cout << ". Difference " << diff << endl;
		bool b = imwrite("Test_out_diff\\Test_out_" + VS[sc].name + "_a.jpg", difference);
		b = imwrite("Test_out\\Test_out_" + VS[sc].name + "_a.jpg", image_out2);
	}


	
	imwrite("Test_out2.jpg", image_out2);
	//cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	//imshow("Display window", image_out); // Show our image inside it.
	//image.
	//cv::waitKey(0); // Wait for a keystroke in the window

	getchar();
	return 0;
}