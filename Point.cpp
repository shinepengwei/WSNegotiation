#include "Point.h"
#include <stdio.h>
#include "MyOut.h"
#include <sstream>
using namespace std;
Point::Point(int num){
	this->num=num;
	for (int i=0;i<ISSUENUM;i++)
	{
		this->val[i]=-1;
	}
}
void Point::setValue(float val,int num){
	this->val[num]=val;
}
float Point::getValue(int num) const{
	return val[num];
}

int Point::getNum() const{
	return num;
}
void Point::randomGenerate(){

	for (int i=0;i<ISSUENUM;i++)
	{
		this->setValue((rand()%100)/10,i);
	}
	Myout::out<<"生成了一个随机点："<<this->printp()<<endl;
}
string Point::printp() const
{
	string str="[";
	if (this->getValue(0)!=-1)
	{
		ostringstream buffer;
		buffer << this->getValue(0);
		string tmpstr = buffer.str();
		
		str=str+tmpstr;
	}
	for (int i=1;i<ISSUENUM;i++)
	{
		if(this->getValue(i)==-1){
			continue;
		}
		ostringstream buffer;
		buffer << this->getValue(i);
		string tmpstr = buffer.str();

		str=str+","+tmpstr;
	}
	str=str+"]";
	return str;
}
