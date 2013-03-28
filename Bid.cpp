#include "Bid.h"
#include "GolbalPameter.h"
#include <sstream>
#include "Result.h"
#include <algorithm>
Bid::Bid(){
	this->beginIssueNum=-1;
	this->endIssueNum=-1;
	for(int i=0;i<ISSUENUM;i++)
	{
		range[i]=new Range(0,10);
	}
	val=-1;
}
Bid::Bid(const Bid * b){
	this->beginIssueNum=b->beginIssueNum;
	this->endIssueNum=b->endIssueNum;
	this->val=b->val;
	for(int i=0;i<ISSUENUM;i++)
	{
		range[i]=new Range(b->getRangeLow(i),b->getRangeUp(i));
	}
}
Bid::Bid(int be,int en)
{
	this->beginIssueNum=be;
	this->endIssueNum=en;
	for(int i=0;i<ISSUENUM;i++)
	{
		range[i]=new Range(0,10);
	}
	val=-1;
}
Bid::~Bid(){
	for (int i=0;i<ISSUENUM;i++)
	{
		if (range[i]!=NULL)
		{
			delete range[i];
		}
	}
}
string Bid::printb() const{
	stringstream buffer;
	stringstream buffer2;
	stringstream buffer1;
	buffer<<this->getVal();
	string str="[value:"+buffer.str()+"]";
	
	for (int i=0;i<endIssueNum+1-beginIssueNum;i++)
	{
		buffer.str("");
		buffer2.str("");
		buffer1.str("");
		int tmp=i+beginIssueNum;
		buffer<<tmp;
		if (range[i]==NULL)
		{
			
			str=str+"["+buffer.str()+":NULL]";
		}
		else{
			buffer1<<range[i]->low;
			buffer2<<range[i]->up;
			str=str+",["+buffer.str()+":"+buffer1.str()+","+buffer2.str()+"]";
		}
	}
	return str;
}
void Bid::setRange(float low,float up,int i){
	range[i]=new Range(low,up);
}
void Bid::setVal(float val){
	this->val=val;
}
float Bid::getRangeLow(int i) const{
	return range[i]->low;
}
float Bid::getRangeUp(int i) const{
	return range[i]->up;
}
float Bid::getVal() const{
	return val;
}
int Bid::getNum() const{
	return endIssueNum-beginIssueNum+1;
}
bool Bid::compareTo(Bid *b) const{
	if (this->val!=b->getVal())
	{
		return false;
	}
	for (int i=0;i<=endIssueNum-beginIssueNum;i++)
	{
		if ((this->range[i]==NULL&&b->range[i]!=NULL)||(this->range[i]!=NULL&&b->range[i]==NULL))
		{
			return false;
		}
		if (this->range[i]!=NULL&&b->range[i]!=NULL)
		{
			if (this->range[i]->up!=b->range[i]->up||this->range[i]->low!=b->range[i]->low)
			{
				return false;
			}
		}
	}
	return true;
}
Result * Bid::getOverlap(const Bid * b) const{
	Result *re=new Result(beginIssueNum,endIssueNum);
	for (int i=0;i<=endIssueNum-beginIssueNum;i++)
	{
		if (range[i]->up<b->range[i]->low||range[i]->low>b->range[i]->up)
		{
			delete re;
			return NULL;
		}
		float tmpup=_cpp_min(range[i]->up,b->range[i]->up);
		float tmplow=_cpp_max(range[i]->low,b->range[i]->low);
		re->setRange(tmplow,tmpup,i);
	}
	re->setVal(this->val+b->val);
	re->setBID(this,b);
	return re;


}