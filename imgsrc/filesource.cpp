#include "filesource.h"


FileSource::FileSource(){
	filelog.open("uav/transfer/uav_gps.log", std::ifstream::in);
	std::string header;
	std::getline(filelog,header);
}

FileSource::~FileSource(){
	filelog.close();
}	

bool FileSource::getImage(std::vector<unsigned char> &imgdata, double &lat, double &lon, double &alt, double &hed){
	std::stringstream ss;
	ss<<"uav/transfer/";

	//Read line from uav_gps.log
	std::string imgname, val;	
	if(std::getline(filelog,imgname,',')){
		std::getline(filelog,val,',');
		lat = std::stod(val);
		std::getline(filelog,val,',');
		lon = std::stod(val);
		std::getline(filelog,val,',');
		alt = std::stod(val);
		std::getline(filelog,val);
		hed = std::stod(val);
	}
	else{
		std::cout<<"End of log!"<<std::endl;
		return false;
	}

	std::ifstream ifs;
	ss<<imgname;
	std::string filepath = ss.str();
	ifs.open(filepath, std::ifstream::binary);
	std::cout<<"Opening image " << imgname << '\n';
	std::cout<<"Latitude: " << lat << ", Longitude: " << lon << ",  Altitude: " << alt << ", Heading: " <<hed << std::endl;
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
		std::cout<<"Error opening image "<<filepath<<std::endl;
		return false;
	}
}	
	
