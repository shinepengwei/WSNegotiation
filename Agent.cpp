#include "MyOut.h"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Agent.h"
#include "Result.h"
using namespace std;
Agent::Agent(string name)
{
	this->name=name;
	currentValue=0;
	group[0][0]=0;
	group[0][1]=1;
	group[1][0]=2;
	group[1][1]=3;
	for (int i=0;i<GROUP_NUM;i++)
	{
		for (int j=0;j<MAX_CS_NUM;j++)
		{
			cs[i][j]=NULL;
		}
	}
	for(i=0;i<GROUP_NUM;i++)
	{
		int index=0;
		for (int xi=0;xi<GROUP_1_NUM;xi++)
		{
			for (int xj=0;xj<5-xi;xj++)
			{
				cs[i][index]=new Constrain(group[i][0],group[i][1],xi+1,name);
				index++;
			}
		}
	}
}
float Agent::getSumSatifyValue(const Point *p,int groupId){
	int group_cs_num[5]={GROUP_1_CS_NUM,GROUP_2_CS_NUM,GROUP_3_CS_NUM,GROUP_4_CS_NUM,GROUP_5_CS_NUM};
	float sumVal=0;
	if (groupId==-1)
	{
		for (int i=0;i<GROUP_NUM;i++)
		{
			for (int j=0;j<group_cs_num[i];j++)
			{
				if (cs[i][j]->satify(p,NOTGROUP))
				{
					sumVal+=cs[i][j]->getVal();
				}
			}
		}
	}
	else{
		for (int j=0;j<group_cs_num[groupId];j++)
		{
			if (cs[groupId][j]->satify(p,ISGROUP))
			{
				sumVal+=cs[groupId][j]->getVal();
			}
		}
	}
	return sumVal;
}
float Agent::accept(const Point *p,float t){
	//��Ҫ�޸�
	return this->accept(p,t,-1);
}
float Agent::accept(const Point *p,float t,int groupId){
	//��Ҫ�޸�
	float sumVal=this->getSumSatifyValue(p,groupId);
	if (sumVal>currentValue)
	{
		Myout::out<<"["<<name<<"]�˻��㷨::Ч��ֵ���ӣ�����["<<currentValue<<"���ӵ�"<<sumVal<<"]"<<endl;
		currentValue=sumVal;
		return currentValue;
	}
	float sub=currentValue-sumVal;
	float gailv=exp(-sub/t);//
	Myout::out<<"["<<name<<"]�˻��㷨::Ч��ֵ��С["<<currentValue<<"���ٵ�"<<sumVal<<"]"<<"�����ܵĸ���Ϊ��"<<gailv<<endl;
	float rr=(float)(rand()%100)/100;
	Myout::out<<"["<<name<<"]�˻��㷨::�����������Ϊ��"<<rr<<endl;
	if (rr<gailv)
	{
		Myout::out<<"["<<name<<"]�˻��㷨::Ч��ֵ��С������"<<endl;
		currentValue=sumVal;
		return currentValue;
	}
	else{
		Myout::out<<"["<<name<<"]�˻��㷨::Ч��ֵ��С���ܾ�"<<endl;
		return -1;
	}
	
	return currentValue;
}
int Agent::getBids(Bid ** rebids,int groupId){
	//��һ������ȡ�漴��
	int group_issue_num[5]={GROUP_1_NUM,GROUP_2_NUM,GROUP_3_NUM,GROUP_4_NUM,GROUP_5_NUM};
	int group_bound[5][2]={GROUP_1_BEGIN,GROUP_1_END,GROUP_2_BEGIN,GROUP_2_END,GROUP_3_BEGIN,GROUP_3_END,GROUP_4_BEGIN,GROUP_4_END,GROUP_5_BEGIN,GROUP_5_END};
	Point * randomPoint[ISSUENUM*200];
	Bid *bids[ISSUENUM*200];


	int number=group_issue_num[groupId]*200;
	int issueNum=group_issue_num[groupId];
	if (groupId==-1)
	{
		issueNum=ISSUENUM;
		number=ISSUENUM*200;
	}
	

	for (int i=0;i<number;i++)//for (int i=0;i<ISSUENUM*200;i++)
	{
		randomPoint[i]=new Point(issueNum);
		randomPoint[i]->randomGenerate();
	}

	float maxValue=0;//�õ����еľֲ����ŵ�����valueֵ
	for (i=0;i<number;i++)
	{
		this->currentValue=0;
		Myout::out<<endl;
		if(groupId!=-1){
			Myout::out<<"GROUP:["<<groupId<<"]��ʼ���ɵ�"<<i<<"��bid"<<endl;
			cout<<"GROUP:["<<groupId<<"]��ʼ���ɵ�"<<i<<"��bid"<<endl;
		}else{
			Myout::out<<"��ʼ���ɵ�"<<i<<"��bid"<<endl;
			cout<<"��ʼ���ɵ�"<<i<<"��bid"<<endl;
		}
		 
		float T=30;
		float curVal=0;
		Point * newp=new Point(issueNum);
		for (int j=0;j<issueNum;j++)
		{
			newp->setValue(randomPoint[i]->getValue(j),j);
		}

		//�ҵ��ֲ����ŵ�
		for (j=0;j<30;j++)
		{
			if (this->accept(newp,T-j,groupId)>0)
			{
				delete randomPoint[i];
				randomPoint[i]=newp;
				newp=Agent::generiatePoint(randomPoint[i],1,issueNum);
			}
			else{
				delete newp;
				newp=Agent::generiatePoint(randomPoint[i],5,issueNum);
			}
		}

		//����bids
		if(groupId==-1){
			bids[i]=new Bid(0,issueNum-1);//������
		}
		else{
			bids[i]=new Bid(group_bound[groupId][0],group_bound[groupId][1]);
		}
		
		float curValue=generiateBids(bids[i],randomPoint[i],groupId);
		if (curValue>maxValue)
		{
			maxValue=curValue;
		}
	}
	Myout::out<<endl<<this->name<<"����bids��"<<endl;
	for (i=0;i<number;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<bids[i]->printb()<<endl;
	}
	//�����е�bids����Ƚϣ�ȥ����ͬ��
	int bidsNum=0;
	for (i=0;i<number;i++)
	{
		if (bids[i] == NULL)
		{
			continue;
		}
		for (int j=i+1;j<number;j++)
		{
			if (bids[j] == NULL)
			{
				continue;
			}
			if (bids[i]->compareTo(bids[j]))
			{
				delete bids[j];
				bids[j]=NULL;
			}
		}
		//���valueС�����value��һ�㣬ɾ��
		if (bids[i]->getVal()<maxValue/2)
		{
			delete bids[i];
			bids[i]=NULL;
			continue;
		}
		rebids[bidsNum]=bids[i];
		bidsNum++;
	}
	Myout::out<<endl<<this->name<<"���bids��"<<endl;
	for (i=0;i<bidsNum;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<rebids[i]->printb()<<endl;
	}
	return bidsNum;
}
//����bid�ĸ���
int Agent::getBids(Bid ** rebids){
	return this->getBids(rebids,-1);
	/*
	//��һ������ȡ�漴��
	Point * randomPoint[ISSUENUM*200];
	int number=ISSUENUM*2;
	Bid *bids[ISSUENUM*200];
	for (int i=0;i<number;i++)//for (int i=0;i<ISSUENUM*200;i++)
	{
		randomPoint[i]=new Point(ISSUENUM);
		randomPoint[i]->randomGenerate();
	}
	//�ڶ���&������������ÿ���漴���ҵ��ֲ����ŵ㣬������һ��bids
	float maxValue=0;//�õ����еľֲ����ŵ�����valueֵ
	for (i=0;i<number;i++)
	{
		this->currentValue=0;
		Myout::out<<endl;
		Myout::out<<"��ʼ���ɵ�"<<i<<"��bid"<<endl;
		cout<<"��ʼ���ɵ�"<<i<<"��bid"<<endl;
		float T=30;
		float curVal=0;
		Point * newp=new Point(ISSUENUM);
		for (int j=0;j<ISSUENUM;j++)
		{
			newp->setValue(randomPoint[i]->getValue(j),j);
		}
		//�ҵ��ֲ����ŵ�
		for (j=0;j<30;j++)
		{
			if (this->accept(newp,T-j)>0)
			{
				delete randomPoint[i];
				randomPoint[i]=newp;
				newp=Agent::generiatePoint(randomPoint[i],1);
			}
			else{
				delete newp;
				newp=Agent::generiatePoint(randomPoint[i],ISSUENUM);
			}
		}
		//����bids
		bids[i]=new Bid(0,ISSUENUM-1);//������
		float curValue=generiateBids(bids[i],randomPoint[i],-1);
		if (curValue>maxValue)
		{
			maxValue=curValue;
		}
	}
	Myout::out<<endl<<this->name<<"����bids��"<<endl;
	for (i=0;i<number;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<bids[i]->printb()<<endl;
	}
	//�����е�bids����Ƚϣ�ȥ����ͬ��
	int bidsNum=0;
	for (i=0;i<number;i++)
	{
		if (bids[i] == NULL)
		{
			continue;
		}
		for (int j=i+1;j<number;j++)
		{
			if (bids[j] == NULL)
			{
				continue;
			}
			if (bids[i]->compareTo(bids[j]))
			{
				delete bids[j];
				bids[j]=NULL;
			}
		}
		//���valueС�����value��һ�㣬ɾ��
		if (bids[i]->getVal()<maxValue/2)
		{
			delete bids[i];
			bids[i]=NULL;
			continue;
		}
		rebids[bidsNum]=bids[i];
		bidsNum++;
	}
	Myout::out<<endl<<this->name<<"���bids��"<<endl;
	for (i=0;i<bidsNum;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<rebids[i]->printb()<<endl;
	}
	return bidsNum;
	*/
	
}
//////////////////////////////////////////////////////////////////////////
//����һ���㣬�ҵ����������Լ����Ȼ���ҵ���ЩԼ�����ص����֣�����һ��bid
float Agent::generiateBids(Bid * bid,const Point * p,int groupId){
	Myout::out<<"���ݵ�:"<<p->printp()<<"����bid"<<endl<<endl;
	int group_cs_num[5]={GROUP_1_CS_NUM,GROUP_2_CS_NUM,GROUP_3_CS_NUM,GROUP_4_CS_NUM,GROUP_5_CS_NUM};
	int group_issue_num[5]={GROUP_1_NUM,GROUP_2_NUM,GROUP_3_NUM,GROUP_4_NUM,GROUP_5_NUM};
	float sumVal=0;
	int issueNum=0;
	if (groupId==-1)
	{
		for (int i=0;i<GROUP_NUM;i++)
		{
			for (int j=0;j<group_cs_num[i];j++)
			{
				if (cs[i][j]->satify(p,NOTGROUP))
				{
					//Myout::out<<"����Լ����"<<cs[i][j]->printc()<<endl;
					cs[i][j]->cutBids(bid,NOTGROUP);
					Myout::out<<"�и��bid��"<<bid->printb()<<endl;
				}
			}
		}
		issueNum=ISSUENUM;
	}else{
		for (int j=0;j<group_cs_num[groupId];j++)
		{
			if (cs[groupId][j]->satify(p,ISGROUP))
			{
				//Myout::out<<"����Լ����"<<cs[i][j]->printc()<<endl;
				cs[groupId][j]->cutBids(bid,ISGROUP);
				Myout::out<<"�и��bid��"<<bid->printb()<<endl;
			}
		}
		issueNum=group_issue_num[groupId];
	}
	
	bid->setVal(this->currentValue);
	Myout::out<<"����һ��bid["<<bid->getVal()<<"]"<<endl;
	for (int i=0;i<issueNum;i++)
	{
		Myout::out<<"      :range["<<i<<"]: ["<<bid->getRangeLow(i)<<","<<bid->getRangeUp(i)<<"]"<<endl;
	}
	return currentValue;

}
//�˻��㷨�У���p����һ���Ŷ�������һ���µ�point��changenum���Ŷ�Ҫ�Ŷ�������ֵ��ÿ���Ŷ�������ӻ��С1
Point *Agent::generiatePoint(const Point * p,int changeNum,int issueNum){
	
	Point * rp=new Point(issueNum);
	for (int i=0;i<issueNum;i++)
	{
		rp->setValue(p->getValue(i),i);
	}
	float span=0.1*changeNum;
	for (int index=0;index<issueNum;index++)
	{
		if (p->getValue(index)<span)
		{
			rp->setValue(p->getValue(index)+span,index);
			continue;
		}
		if (p->getValue(index)>10-span)
		{
			rp->setValue(p->getValue(index)-span,index);
			continue;
		}
		int b=rand()%3;
		if (b==1)
		{
			rp->setValue(p->getValue(index)+span,index);
		}else if (b==2)
		{
			rp->setValue(p->getValue(index)-span,index);
		}
	}
	Myout::out<<"  [�˻��㷨-���Ե�����]���ݵ㣺"+p->printp()<<"�����Ŷ������ɵ�:"<<rp->printp()<<endl;
	return rp;
}
Point *Agent::generiatePoint(const Point * p,int changeNum){
	return Agent::generiatePoint(p,changeNum,ISSUENUM);
}

//for MA��deal identification
Result * Agent::dealBids(Bid ** bidsCa, int num_ca,Bid ** bidsSa,int num_sa){
	Result *result;
	int maxValue=0;
	Result * maxResult=NULL;
	for (int i=0;i<num_ca;i++)
	{
		
		for (int j=0;j<num_sa;j++)
		{
			cout<<"CA��"<<i<<"��BIDs��Sa��"<<j<<"���Ƚ�"<<endl;
			Myout::out<<"CA��"<<i<<"��BIDs��Sa��"<<j<<"���Ƚ�"<<endl;
			result=bidsCa[i]->getOverlap(bidsSa[j]);
			if (result!=NULL)
			{
				cout<<"�ɹ�������һ��result"<<result->printb()<<endl;
				Myout::out<<"�ɹ�������һ��result"<<result->printb()<<endl;
				if (maxValue<result->getVal())
				{
					maxValue=result->getVal();
					if (maxResult!=NULL)
					{
						delete maxResult;
					}
					maxResult=result;
				}
			}
		}
		delete bidsCa[i];
	}
	for (i=0;i<num_sa;i++)
	{
		delete bidsSa[i];
	}
	if (maxValue==0)
	{
		Myout::out<<"�㷨2ʧ��"<<endl;
	}
	
	Myout::out<<"�㷨2�����"<<endl<<maxResult->printb()<<endl;
	return maxResult;
	//
}
