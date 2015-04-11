#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class ImageSource {

	public: 
		ImageSource(){}
		virtual ~ImageSource(){}
	
		virtual bool getImage(std::vector<unsigned char> &data, double &lat, double &lon, double &alt, double &hd) = 0;
		int height, width;		
};
