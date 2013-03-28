#include "Constrain.h"
#include <iostream>
#include "MyOut.h"
using namespace std;
#include <stdio.h> 
#include <sstream>
#include <string>
/************************************************************************/
/* 
每一个约束对应于一个协商议题子集
begin_num:		某一个协商议题子集范围
end_num:		某一个协商议题子集范围
range_num:		此约束是几维约束
若此约束是｛2,3｝议题子集，begin_num=2,end_num=3
若此约束是一个一维约束，range_num=1。
具体和哪一维相关随机生成

                                                                     */
/************************************************************************/
Constrain::Constrain(int begin_num,int end_num,int range_num,string name){
	this->beginIssueNum=begin_num;
	this->endIssueNum=end_num;
	for (int i=0;i<MAXGROUPISSUENUM;i++)
	{
		range[i]=NULL;
	}
	genernateRandom(range_num);
	Myout::out<<name<<"生成了"<<range_num<<"维约束，约束属于议题["<<begin_num<<","<<end_num<<"];"<<endl;
	for (i=0;i<end_num+1-begin_num;i++)
	{
		if (range[i]==NULL)
		{
			Myout::out<<"    议题"<<i+begin_num<<"无约束；"<<endl;
		}
		else{
			Myout::out<<"    议题"<<i+begin_num<<"约束范围：["<<range[i]->low<<","<<range[i]->up<<"];"<<endl;
		}
	}
	Myout::out<<"    约束value："<<val<<endl;
}
Constrain::~Constrain()
{
	for (int i=0;i<MAXGROUPISSUENUM;i++)
	{
		if (range[i]!=NULL)
		{
			delete range[i];
		}
	}
}
string Constrain::printc() const{
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
void Constrain::genernateRandom(int range_num){
//在每个范围中生成一个[0,100]的点除以10，作为范围的中心的点。
//生成一个0~70的点除以20，作为范围的宽度半径。
	int issueNum=endIssueNum-beginIssueNum+1;
	//根据issuenum和range_num先随机选择约束的议题
	int cur_isuNum=0;
	while (true)
	{
		int index=rand()%issueNum;
		if (range[index] == NULL)
		{
			float mid=(float)(rand()%101)/10;
			float r=(float)(rand()%36)/10;
			float low=mid-r;
			float up=mid+r;
			if (low<0)
			{
				low=0;
			}
			if (up>10)
			{
				up=10;
			}
			range[index]=new Range(low,up);
			cur_isuNum++;
			if (cur_isuNum == range_num)
			{
				break;
			}
		}
	}
	//生成value
	val=rand()%(100*range_num+1);
}
void Constrain::setRange(float range_low,float range_up,int range_num){
	this->range[range_num-this->beginIssueNum]=new Range(range_low,range_up);
}
float Constrain::getVal() const {
	return val;
}
bool Constrain::satify(const Point *p,bool isGruop){
	bool ret=true;;
	if(isGruop){
		//分组，不考虑从第几个议题开始
		for (int i=0;i<p->getNum();i++)
		{
			if (range[i] == NULL)
			{
				continue;
			}
			if (p->getValue(i)<this->range[i]->low||p->getValue(i)>this->range[i]->up)
			{
				ret=false;
			}
		}
	}
	else
	{
		//算法1、2，不考虑分组
		for (int i=0;i<=endIssueNum-beginIssueNum;i++)
		{
			if (range[i] == NULL)
			{
				continue;
			}
			if (p->getValue(i+beginIssueNum)<this->range[i]->low||p->getValue(i+beginIssueNum)>this->range[i]->up)
			{
				ret= false;
			}
		}
	}
	if (ret)
	{
		Myout::out<<"点："<<p->printp()<<"满足约束："<<this->printc()<<endl;
	}
	else{
		Myout::out<<"点："<<p->printp()<<"不满足约束："<<this->printc()<<endl;
	}
	return ret;
}
//取bid和此约束的range交集
void Constrain::cutBids(Bid * bids,bool isGruop){
	if(isGruop){
		//分组，不考虑从第几个议题开始
		for (int i=0;i<bids->getNum();i++)
		{
			if (range[i]== NULL)
			{
				continue;
			}
			float bidlow=bids->getRangeLow(i);
			float bidup=bids->getRangeUp(i);
			if (bidlow<this->getRangeLow(i))
			{
				bidlow=this->getRangeLow(i);
			}
			if (bidup>this->getRangeUp(i))
			{
				bidup=this->getRangeUp(i);
			}
			bids->setRange(bidlow,bidup,i);
		}
	}
	else
	{
		//算法1、2，不考虑分组
		for (int i=0;i<=endIssueNum-beginIssueNum;i++)
		{
			if (range[i] == NULL)
			{
				continue;
			}
			float bidlow=bids->getRangeLow(i+beginIssueNum);
			float bidup=bids->getRangeUp(i+beginIssueNum);
			if (bidlow<this->getRangeLow(i))
			{
				bidlow=this->getRangeLow(i);
			}
			if (bidup>this->getRangeUp(i))
			{
				bidup=this->getRangeUp(i);
			}
			bids->setRange(bidlow,bidup,i+beginIssueNum);
		}
	}
}
float Constrain::getRangeLow(int i) const{
	return range[i]->low;
}
float Constrain::getRangeUp(int i) const{
		return range[i]->up;
}