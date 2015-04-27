#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

#define SENS_WIDTH 24
#define SENS_HEIGHT 18
#define FOCAL_LENGTH 50
namespace groundvision{	

	void whiteBalance(cv::Mat& img);

	void makeTarget(cv::Mat &image, cv::Point pnt1, cv::Point pnt2);
	cv::Point map(cv::Point pnt, double size_factor);
	double calculateArea(int w, int h, std::vector<cv::Point> &poly, double altitude);	
	void geolocate(int x, int y, int w, int h, double imlat, double imlon, double altitude, double heeading, double &targlat, double& targlon);
	void centroid(std::vector<cv::Point> &poly, int& xc, int&yc);	
}
