#include "filesource.h"


FileSource::FileSource(){
	filelog.open("uav_gps.log", std::ifstream::in);
	std::string header;
	std::getline(filelog,header);
}

FileSource::~FileSource(){
	filelog.close();
}	

bool FileSource::getImage(std::vector<unsigned char> &imgdata, struct gpsinfo &imgps){
	//Read line from uav_gps.log
	std::string imgname, val;	
	if(std::getline(filelog,imgname,',')){
		std::getline(filelog,val,',');
		imgps.latitude = std::stod(val);
		std::getline(filelog,val,',');
		imgps.longitude = std::stod(val);
		std::getline(filelog,val,',');
		imgps.altitude = std::stod(val);
		std::getline(filelog,val,',');
		imgps.heading = std::stod(val);
		std::getline(filelog,imgps.timestamp);
	}
	else{
		std::cout<<"End of log!"<<std::endl;
		return false;
	}

	std::ifstream ifs;
	ifs.open(imgname, std::ifstream::binary);
	std::cout<<"Opening image " << imgname << '\n';
	if(ifs) {
		ifs.seekg(0,ifs.end);
		std::streampos size = ifs.tellg();
		imgdata.resize(size);

		ifs.seekg(0,ifs.beg);
		ifs.read((char*)&imgdata[0],size);
		ifs.close();
		return true;
	}
	else{
		std::cout<<"Error opening image "<<imgname<<std::endl;
		return false;
	}
}	
	
