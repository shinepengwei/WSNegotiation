#include "Constrain.h"
#include <iostream>
#include "MyOut.h"
using namespace std;
#include <stdio.h> 
#include <sstream>
#include <string>
/************************************************************************/
/* 
ÿһ��Լ����Ӧ��һ��Э�������Ӽ�
begin_num:		ĳһ��Э�������Ӽ���Χ
end_num:		ĳһ��Э�������Ӽ���Χ
range_num:		��Լ���Ǽ�άԼ��
����Լ���ǣ�2,3�������Ӽ���begin_num=2,end_num=3
����Լ����һ��һάԼ����range_num=1��
�������һά����������

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
	Myout::out<<name<<"������"<<range_num<<"άԼ����Լ����������["<<begin_num<<","<<end_num<<"];"<<endl;
	for (i=0;i<end_num+1-begin_num;i++)
	{
		if (range[i]==NULL)
		{
			Myout::out<<"    ����"<<i+begin_num<<"��Լ����"<<endl;
		}
		else{
			Myout::out<<"    ����"<<i+begin_num<<"Լ����Χ��["<<range[i]->low<<","<<range[i]->up<<"];"<<endl;
		}
	}
	Myout::out<<"    Լ��value��"<<val<<endl;
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
//��ÿ����Χ������һ��[0,100]�ĵ����10����Ϊ��Χ�����ĵĵ㡣
//����һ��0~70�ĵ����20����Ϊ��Χ�Ŀ�Ȱ뾶��
	int issueNum=endIssueNum-beginIssueNum+1;
	//����issuenum��range_num�����ѡ��Լ��������
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
	//����value
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
		//���飬�����Ǵӵڼ������⿪ʼ
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
		//�㷨1��2�������Ƿ���
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
		Myout::out<<"�㣺"<<p->printp()<<"����Լ����"<<this->printc()<<endl;
	}
	else{
		Myout::out<<"�㣺"<<p->printp()<<"������Լ����"<<this->printc()<<endl;
	}
	return ret;
}
//ȡbid�ʹ�Լ����range����
void Constrain::cutBids(Bid * bids,bool isGruop){
	if(isGruop){
		//���飬�����Ǵӵڼ������⿪ʼ
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
		//�㷨1��2�������Ƿ���
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