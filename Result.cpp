#include "Result.h"
#include "GolbalPameter.h"
#include <sstream>
Result::Result()
{
	this->beginIssueNum=-1;
	this->endIssueNum=-1;
	for(int i=0;i<ISSUENUM;i++)
	{
		range[i]=new Range(0,10);
	}
	val=-1;
	caBid=NULL;
	saBid=NULL;
}
Result::Result(int be,int en){
	this->beginIssueNum=be;
	this->endIssueNum=en;
	for(int i=0;i<ISSUENUM;i++)
	{
		range[i]=new Range(0,10);
	}
	val=-1;
	saBid=NULL;
	caBid=NULL;

}
Result::~Result(){
	for (int i=0;i<ISSUENUM;i++)
	{
		if (range[i]!=NULL)
		{
			delete range[i];
		}
	}
	if (saBid!=NULL)
	{
		delete saBid;
	}
	if (caBid!=NULL)
	{
		delete caBid;
	}
}
void Result::setBID(const Bid * caBid,const Bid * saBid){
	this->caBid=new Bid(caBid);
	this->saBid=new Bid(saBid);
}
void Result::setRange(float low,float up,int i){
	this->range[i]->setNum(low,up);
}
void Result::setVal(float val){
	this->val=val;
}
float Result::getVal() const{
	return val;
}
string Result::printb() const{
	stringstream buffer;
	stringstream buffer2;
	stringstream buffer1;
	buffer<<this->getVal();
	string str="结果[value:"+buffer.str()+"]";
	
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
	str=str+"\n包含的Bid包括：\n";
	str=str+"CA BID:"+caBid->printb()+"\n";
	str=str+"SA BID:"+saBid->printb()+"\n";
	return str;
}

