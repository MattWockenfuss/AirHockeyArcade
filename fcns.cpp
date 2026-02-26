#include "fcns.hpp"

std::vector<int> getIndexes(int center, int size){
	// create indexes
	std::vector<int> indexes = {center-1,center,center+1};
	// wrap around if needed
	if(indexes[0]<0)
		indexes[0] = size-1;
	if(indexes[2]>=size)
		indexes[2] = 0;
	
	return indexes;
}
	
std::vector<double> getAnimXYZ(float frame){
	// 0s   -> (-192,56,80)
	// 0.5s -> (-96,48,96)
	// 1.5s -> (96,32,128)
	// 2.5s -> (288,48,96)
	// 3s   -> (384,56,80)
	
	double x = -192.0 + (192*frame);
	if(frame<1.5){ // distance is smaller for element left of center
		x = -144.0 + (160*frame);
	}
	double y = 32 + 16*sqrt( pow(1.5-frame,2.0));
	double z = 128 - 32*sqrt( pow(1.5-frame,2.0));
	std::vector<double> xyz = {x,y,z};
	return xyz;
}
