#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

#include <thread>
#include <opencv2/opencv.hpp>

//Libraries
#include "filesource.h"
#include "imgutils.h"

#define NONE 0
#define TARGET_PICK 1
#define AREA_SELECT 2
#define SIZE_FACTOR 0.5

int mode = 0;
bool box = false;
cv::Point pnt;
cv::Mat image, preview, draw;
std::vector<cv::Point> pointlist;
struct gpsinfo imgps;

void windowClick(int event, int x, int y, int, void*){
	int xc, yc;
	cv::Point tmp = cv::Point(x,y);

	if (mode == TARGET_PICK) {
		if( event == cv::EVENT_LBUTTONDOWN && !box){
			pnt = tmp;
			box = true;
		}	
		if( event == cv::EVENT_MOUSEMOVE && box){
			draw = preview.clone();
			cv::rectangle(draw,pnt,tmp,cv::Scalar(255,255,255));	
			cv::imshow("Main",draw);
		}
		if( event ==cv::EVENT_LBUTTONUP && box){
			if( x-pnt.x < 5 || y-pnt.y < 5){
				cv::imshow("Main",preview);
				box = false;
				return;
			}
			box = false;
			cv::Point topleft = groundvision::map(pnt,SIZE_FACTOR);
			cv::Point bottomright = groundvision::map(tmp,SIZE_FACTOR);
			xc = (int)((topleft.x+bottomright.x)/2);
			yc = (int)((topleft.y+bottomright.y)/2);
			//groundvision::geolocate();
			std::cout<<"Target located at: "<<std::fixed<<imgps.latitude << ", " <<imgps.longitude<<std::endl;
			groundvision::makeTarget(image,topleft,bottomright);
			cv::imshow("Main",preview);
		}
	} /*else if (mode == AREA_SELECT){
		if( event ==cv::EVENT_LBUTTONDOWN){
			cv::Point mapped = groundvision::map(tmp, SIZE_FACTOR);

			if(pointlist.size()==0)
				draw = preview.clone();

			cv::circle(draw,tmp, 3, cv::Scalar(0,0,0), -1);
			cv::imshow("Main",draw);
			pointlist.push_back(mapped);
		}
		else if (event == cv::EVENT_RBUTTONDOWN){
			double area = groundvision::calculateArea();
			groundvision::centroid(pointlist,xc,yc);
			groundvision::geolocate();
			std::cout<<"Area: " << area << " located at: "<<std::endl;	
			cv::imshow("Main",preview);
			pointlist.clear();
		}
	
	}*/
	else
		return;
}

int main(){
	cv::Mat jpg;
	char option;
	bool image_ok;
	int button;
	std::vector<unsigned char> buffer;
	ImageSource* src;

	std::cout.precision(9);
	std::cout<<"Opening files from the saved directory"<<std::endl;

	cv::namedWindow("Main", cv::WINDOW_AUTOSIZE);
	cv::moveWindow("Main",0,0);

	src = new FileSource();	
	cv::setMouseCallback("Main",windowClick,0);
 	
	image_ok = src->getImage(buffer, imgps);

	while(image_ok){
		jpg = cv::Mat(buffer);
		image = cv::imdecode(jpg,1);
		groundvision::whiteBalance(image);		

		cv::resize(image,preview,cv::Size(),SIZE_FACTOR,SIZE_FACTOR,cv::INTER_NEAREST);
		cv::imshow("Main",preview);
	
		do {	
			button = cv::waitKey(0);

			if(button == 'q'){
				mode = TARGET_PICK;
				std::cout<<"Box select target"<<std::endl;
			}
			/*else if (button == 'p'){
				mode = AREA_SELECT;	
				std::cout<<"Click polygon corners" << std::endl;
				pointlist.clear();
			}*/
		}while(button !='e');
	
		image_ok = src->getImage(buffer, imgps);
	}
	std::cout<<"Done getting Images"<<std::endl;	
	return 0;
}
