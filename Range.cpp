#include "Range.h"
#include <iostream>
#include "MyOut.h"
using std::cout;
using std::endl;
void Range::setNum(const float low,const float up){
	this->low=low;
	this->up=up;
}
Range::Range(float low,float up){
	setNum(low,up);
}
float Range::getLow(){
	return low;
}
float Range::getUp(){
	return up;
}