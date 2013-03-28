#ifndef POINT_H
#define POINT_H
#include <iostream>
#include <string>
#include "GolbalPameter.h"
using namespace std;
class Point{
private:
	int num;
	float val[ISSUENUM];
public:
	Point(int num);
	void setValue(float,int);
	 float  getValue(int) const;
	 int  getNum() const;
	 void randomGenerate();
	 string printp() const;
};
#endif