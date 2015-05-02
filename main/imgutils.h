#include <string>
#include <vector>
#include <opencv2/opencv.hpp>
#include <zbar.h>

#define SENS_WIDTH 3.6
#define SENS_HEIGHT 2.7	
#define FOCAL_LENGTH 4.4

namespace groundvision{	

	void whiteBalance(cv::Mat& img);

	void makeTarget(cv::Mat &image, cv::Point pnt1, cv::Point pnt2);
	cv::Point map(cv::Point pnt, double size_factor);
	double calculateArea(int w, int h, std::vector<cv::Point> &poly, double altitude);	
	void geolocate(int x, int y, int w, int h, double imlat, double imlon, double altitude, double heading, double &targlat, double& targlon);
	void centroid(std::vector<cv::Point> &poly, int& xc, int&yc);	

	void readQR(cv::Mat &roi);
}
