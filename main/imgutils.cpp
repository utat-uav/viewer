#include <string>
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>
#include "imgutils.h"

namespace groundvision{
	cv::Mat channels[3];

	void whiteBalance(cv::Mat& img){
		float r[] = {0,256};
		int hsize = 256;
		int j;
		int total = img.cols*img.rows;;
		float n, min, max;
		float qmin = total*0.05f;
		float qmax = total*0.95f;

		const float* range = {r};
		cv::Mat hist;

		cv::split(img,channels);
		
		for (int i = 0; i<3;i++){
			cv::calcHist(&channels[i], 1,0, cv::Mat(), hist,1,&hsize,&range );
			n = 0.0f;
			j = 0;
			min = 0.0f;
			while(n < qmin){
				n += hist.at<float>(j++);
				min += 1.0f;
			}

			n = (float)total;
			j = hsize;
			max = 255.0f;
			while(n > qmax){
				n -= hist.at<float>(j--);
				max -= 1.0f;	
			}
			channels[i] = (channels[i]-min)*255.0f/(max-min); 
		}
		cv::merge(channels,3,img);
	}
	
	void makeTarget(cv::Mat &image, cv::Point pnt1, cv::Point pnt2){
			cv::Rect targetroi = cv::Rect(pnt1.x, pnt1.y, pnt2.x-pnt1.x, pnt2.y-pnt1.y);
			cv::Mat roiImg = image(targetroi);
			cv::namedWindow("Target", cv::WINDOW_AUTOSIZE);
			cv::moveWindow("Target",500,0);
			cv::imshow("Target",roiImg);
			cv::waitKey(0);
			cv::destroyWindow("Target");
	}
	
	cv::Point map(cv::Point pnt, double size_factor){
		double fac = 1/size_factor;
		return cv::Point(pnt.x*fac, pnt.y*fac);
	}

	void geolocate(int x, int y, int w, int h, double imlat, double imlon, double altitude, double heading, double& targlat, double& targlon){
		double theta, beta;
		double alphax, alphay, fovheight, fovwidth, imgheight, imgwidth;
		double xd, yd, d;
		double pi = acos(-1);
		double R = 6371000;
		
		alphax = 2*atan2(SENS_WIDTH,2*FOCAL_LENGTH);
		alphay = 2*atan2(SENS_HEIGHT,2*FOCAL_LENGTH);
		fovwidth = 2*altitude*tan(alphax/2);
		fovheight = 2*altitude*tan(alphay/2);
		imgwidth = double(w);
		imgheight = double(h);

		xd = (double(x)-imgwidth/2)/imgwidth*fovwidth;
		yd = (double(y)-imgheight/2)/imgheight*fovheight;
		beta = (yd==0) ? 0: atan2(xd,yd);

		if (xd>=0) beta = pi-beta;
		else beta = -pi-beta;

		theta = beta-heading;
		d = sqrt(pow(xd,2) + pow(yd,2));
		targlat = asin( sin(imlat)*cos(d/R) + cos(imlat)*sin(d/R)*cos(theta));
		targlon = (imlon + atan2(sin(theta)*sin(d/R)*cos(imlat), cos(d/R)-sin(imlat)*sin(targlat)));


	}	
	double calculateArea(int w, int h, std::vector<cv::Point> &poly, double altitude){
		double fovwidth, fovheight, imgwidth, imgheight, alphax, alphay;
		double imgarea;
		alphax = 2*atan2(SENS_WIDTH,2*FOCAL_LENGTH);
		alphay = 2*atan2(SENS_HEIGHT,2*FOCAL_LENGTH);
		fovwidth = 2*altitude*tan(alphax/2);
		fovheight = 2*altitude*tan(alphay/2);
		imgwidth = double(w);
		imgheight = double(h);
		
		imgarea = contourArea(poly);
		return imgarea*fovwidth*fovheight/imgwidth/imgheight;
	}
	
	void centroid(std::vector<cv::Point> &poly, int &xc, int &yc){

		double xtmp = 0.0;
		double ytmp = 0.0;
		for (std::vector<cv::Point>::size_type i = 0; i<poly.size(); i++){
			xtmp = xtmp + poly[i].x;
			ytmp = ytmp + poly[i].y;
		}	

		xtmp = xtmp/poly.size();
		ytmp = ytmp/poly.size();
	        xc = (int) xtmp;
		yc = (int) ytmp;	
	}


}