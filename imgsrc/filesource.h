#include "imgsource.h"
#include <string>
#include <iostream>
#include <fstream>

#define PREFIX "uav/transfer/im"

class FileSource: public ImageSource {

	public: 
		FileSource();
		~FileSource();	
		
		bool getImage(std::vector<unsigned char> &data, double &lat, double &lon, double &alt, double &hed);
	private:
		std::ifstream filelog;
};
