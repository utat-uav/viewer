#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include "gpsinfo.h"

class ImageSource {

	public: 
		ImageSource(){}
		virtual ~ImageSource(){}
	
		virtual bool getImage(std::vector<unsigned char> &data, struct gpsinfo &imgps) = 0;
		int height, width;		
};
