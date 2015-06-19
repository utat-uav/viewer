#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <opencv2/opencv.hpp>
#include <iomanip>
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
	
	std::string makeTarget(cv::Mat &image, cv::Point pnt1, cv::Point pnt2, int &n){
		cv::Rect targetroi = cv::Rect(pnt1.x, pnt1.y, pnt2.x-pnt1.x, pnt2.y-pnt1.y);
		cv::Mat roiImg = image(targetroi);
		cv::namedWindow("Target", cv::WINDOW_AUTOSIZE);
		cv::moveWindow("Target",500,0);
		cv::imshow("Target",roiImg);
		cv::waitKey(0);
		std::stringstream file;
		std::vector<int> jpeg;
		jpeg.push_back(CV_IMWRITE_JPEG_QUALITY);
		jpeg.push_back(90);
		file<<"target"<<std::setfill('0')<<std::setw(2)<<++n<<".jpg";	
		cv::imwrite(file.str(),roiImg,jpeg);
		cv::destroyWindow("Target");
		readQR(roiImg);
		return file.str();
	}
	
	cv::Point map(cv::Point pnt, double size_factor){
		double fac = 1/size_factor;
		return cv::Point(pnt.x*fac, pnt.y*fac);
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

	void readQR(cv::Mat &roi){
		cv::Mat gryscl;
		cv::cvtColor(roi,gryscl,CV_RGB2GRAY);
		unsigned char* data = (unsigned char*)(gryscl.data);

		zbar::Image im(gryscl.cols, gryscl.rows, "Y800", data, gryscl.cols*gryscl.rows);
		zbar::ImageScanner scanner;
		scanner.set_config(zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE,1);
		scanner.scan(im);
		for(zbar::Image::SymbolIterator symbol = im.symbol_begin(); symbol!=im.symbol_end(); ++symbol){
			std::cout<<"Found QR Code: " <<symbol->get_data()<<std::endl;
		}
	}
}
/*	void geolocate(struct gpsinfo imgps , double[2] &coord){
		double theta, beta;
		double alphax, alphay, fovheight, fovwidth, imgheight, imgwidth;
		double xd, yd, d;
		double pi = acos(-1);
		double R = 6371000;
		double lat, lon, tlat, tlon, alt;
	  	lat = imgps.latitude*pi/180;
		lon = imgps.longitude*pi/180;

		alphax = 2*atan2(SENS_WIDTH,2*FOCAL_LENGTH);
		alphay = 2*atan2(SENS_HEIGHT,2*FOCAL_LENGTH);
		fovwidth = 2*imgps.altitude*tan(alphax/2);
		fovheight = 2*imgps.altitude*tan(alphay/2);
		imgwidth = double(w);
		imgheight = double(h);

		alt = imgps.altitude-128;
		xd = (double(x)-imgwidth/2)/imgwidth*fovwidth;
		yd = (double(y)-imgheight/2)/imgheight*fovheight;
		beta = (yd==0) ? 0: atan2(xd,yd);

		if (xd>=0) beta = pi-beta;
		else beta = -pi-beta;

		theta = beta-heading*pi/180;
		d = sqrt(pow(xd,2) + pow(yd,2));
		tlat = asin( sin(lat)*cos(d/R) + cos(lat)*sin(d/R)*cos(theta));
		tlon = (lon + atan2(sin(theta)*sin(d/R)*cos(lat), cos(d/R)-sin(lat)*sin(tlat)));
		coord[0] = targlat*180/pi;
		coord[1] = targlon*180/pi;
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
	*/
