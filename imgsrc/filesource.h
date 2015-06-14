#include "imgsource.h"
#include <string>
#include <iostream>
#include <fstream>

#define PREFIX "im"

class FileSource: public ImageSource {

	public: 
		FileSource();
		~FileSource();	
		
		bool getImage(std::vector<unsigned char> &data, struct gpsinfo &pos);
	private:
		std::ifstream filelog;
};
