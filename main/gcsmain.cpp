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


void makeTarget(int event, int x, int y, int, void*){
	if( event != cv::EVENT_LBUTTONDOWN )      return;

	std::cout<<"Target Noted!"<<std::endl;
}

int main(){
	cv::Mat jpg, image, prvw;
	char option;
	bool image_ok;
	int button;
	double latitude, longitude, altitude, heading;
	std::vector<unsigned char> buffer;
	ImageSource* src;

	std::cout<<"Opening files from the saved directory"<<std::endl;

	cv::namedWindow("main", cv::WINDOW_AUTOSIZE);

	src = new FileSource();	
	cv::setMouseCallback("main",makeTarget,0);
 	
	image_ok = src->getImage(buffer, latitude, longitude, altitude, heading );		

	while(image_ok){
		std::cout<<"Latitude: "<<latitude<< "  Longitude: "<< longitude;
		std::cout<<"  Altitude"<<altitude<<"  Heading"<<heading<<std::endl;
		jpg = cv::Mat(buffer);
		image = cv::imdecode(jpg,1);
		groundvision::whiteBalance(image);		

		//cv::resize(image,prvw,cv::Size(),0.25,0.25,cv::INTER_NEAREST);
		//cv::imshow("main",prvew);
		
		cv::imshow("main", image);
	
		button = cv::waitKey(0);
		if(button == 'q') {
			std::cout<<"Remember image"<<std::endl;
		}	
	
		image_ok = src->getImage(buffer,latitude, longitude, altitude, heading);
	}
	std::cout<<"Done getting Images"<<std::endl;	
	return 0;
}
