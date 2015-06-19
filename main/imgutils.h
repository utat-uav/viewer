#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <zbar.h>

#define SENS_WIDTH 
#define SENS_HEIGHT 2.7	
#define FOCAL_LENGTH 4.4

namespace groundvision{	

	void whiteBalance(cv::Mat& img);

	std::string makeTarget(cv::Mat &image, cv::Point pnt1, cv::Point pnt2, int &n);
	cv::Point map(cv::Point pnt, double size_factor);
	void centroid(std::vector<cv::Point> &poly, int& xc, int&yc);	
	void readQR(cv::Mat &roi);
}
