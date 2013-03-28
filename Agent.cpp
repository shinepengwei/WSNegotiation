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
	//需要修改
	return this->accept(p,t,-1);
}
float Agent::accept(const Point *p,float t,int groupId){
	//需要修改
	float sumVal=this->getSumSatifyValue(p,groupId);
	if (sumVal>currentValue)
	{
		Myout::out<<"["<<name<<"]退火算法::效用值增加，接受["<<currentValue<<"增加到"<<sumVal<<"]"<<endl;
		currentValue=sumVal;
		return currentValue;
	}
	float sub=currentValue-sumVal;
	float gailv=exp(-sub/t);//
	Myout::out<<"["<<name<<"]退火算法::效用值减小["<<currentValue<<"减少到"<<sumVal<<"]"<<"，接受的概率为："<<gailv<<endl;
	float rr=(float)(rand()%100)/100;
	Myout::out<<"["<<name<<"]退火算法::产生的随机数为："<<rr<<endl;
	if (rr<gailv)
	{
		Myout::out<<"["<<name<<"]退火算法::效用值减小，接受"<<endl;
		currentValue=sumVal;
		return currentValue;
	}
	else{
		Myout::out<<"["<<name<<"]退火算法::效用值减小，拒绝"<<endl;
		return -1;
	}
	
	return currentValue;
}
int Agent::getBids(Bid ** rebids,int groupId){
	//第一步，抽取随即点
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

	float maxValue=0;//好到所有的局部最优点的最大value值
	for (i=0;i<number;i++)
	{
		this->currentValue=0;
		Myout::out<<endl;
		if(groupId!=-1){
			Myout::out<<"GROUP:["<<groupId<<"]开始生成第"<<i<<"个bid"<<endl;
			cout<<"GROUP:["<<groupId<<"]开始生成第"<<i<<"个bid"<<endl;
		}else{
			Myout::out<<"开始生成第"<<i<<"个bid"<<endl;
			cout<<"开始生成第"<<i<<"个bid"<<endl;
		}
		 
		float T=30;
		float curVal=0;
		Point * newp=new Point(issueNum);
		for (int j=0;j<issueNum;j++)
		{
			newp->setValue(randomPoint[i]->getValue(j),j);
		}

		//找到局部最优点
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

		//生成bids
		if(groupId==-1){
			bids[i]=new Bid(0,issueNum-1);//不分组
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
	Myout::out<<endl<<this->name<<"生成bids："<<endl;
	for (i=0;i<number;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<bids[i]->printb()<<endl;
	}
	//将所有的bids互相比较，去掉相同的
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
		//如果value小于最大value的一般，删除
		if (bids[i]->getVal()<maxValue/2)
		{
			delete bids[i];
			bids[i]=NULL;
			continue;
		}
		rebids[bidsNum]=bids[i];
		bidsNum++;
	}
	Myout::out<<endl<<this->name<<"提出bids："<<endl;
	for (i=0;i<bidsNum;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<rebids[i]->printb()<<endl;
	}
	return bidsNum;
}
//返回bid的个数
int Agent::getBids(Bid ** rebids){
	return this->getBids(rebids,-1);
	/*
	//第一步，抽取随即点
	Point * randomPoint[ISSUENUM*200];
	int number=ISSUENUM*2;
	Bid *bids[ISSUENUM*200];
	for (int i=0;i<number;i++)//for (int i=0;i<ISSUENUM*200;i++)
	{
		randomPoint[i]=new Point(ISSUENUM);
		randomPoint[i]->randomGenerate();
	}
	//第二步&第三部，根据每个随即点找到局部最优点，并生成一个bids
	float maxValue=0;//好到所有的局部最优点的最大value值
	for (i=0;i<number;i++)
	{
		this->currentValue=0;
		Myout::out<<endl;
		Myout::out<<"开始生成第"<<i<<"个bid"<<endl;
		cout<<"开始生成第"<<i<<"个bid"<<endl;
		float T=30;
		float curVal=0;
		Point * newp=new Point(ISSUENUM);
		for (int j=0;j<ISSUENUM;j++)
		{
			newp->setValue(randomPoint[i]->getValue(j),j);
		}
		//找到局部最优点
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
		//生成bids
		bids[i]=new Bid(0,ISSUENUM-1);//不分组
		float curValue=generiateBids(bids[i],randomPoint[i],-1);
		if (curValue>maxValue)
		{
			maxValue=curValue;
		}
	}
	Myout::out<<endl<<this->name<<"生成bids："<<endl;
	for (i=0;i<number;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<bids[i]->printb()<<endl;
	}
	//将所有的bids互相比较，去掉相同的
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
		//如果value小于最大value的一般，删除
		if (bids[i]->getVal()<maxValue/2)
		{
			delete bids[i];
			bids[i]=NULL;
			continue;
		}
		rebids[bidsNum]=bids[i];
		bidsNum++;
	}
	Myout::out<<endl<<this->name<<"提出bids："<<endl;
	for (i=0;i<bidsNum;i++)
	{
		Myout::out<<" ["<<i<<"]:"<<rebids[i]->printb()<<endl;
	}
	return bidsNum;
	*/
	
}
//////////////////////////////////////////////////////////////////////////
//根据一个点，找到满足的所有约束，然后找到这些约束的重叠部分，生成一个bid
float Agent::generiateBids(Bid * bid,const Point * p,int groupId){
	Myout::out<<"根据点:"<<p->printp()<<"生成bid"<<endl<<endl;
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
					//Myout::out<<"满足约束："<<cs[i][j]->printc()<<endl;
					cs[i][j]->cutBids(bid,NOTGROUP);
					Myout::out<<"切割后bid："<<bid->printb()<<endl;
				}
			}
		}
		issueNum=ISSUENUM;
	}else{
		for (int j=0;j<group_cs_num[groupId];j++)
		{
			if (cs[groupId][j]->satify(p,ISGROUP))
			{
				//Myout::out<<"满足约束："<<cs[i][j]->printc()<<endl;
				cs[groupId][j]->cutBids(bid,ISGROUP);
				Myout::out<<"切割后bid："<<bid->printb()<<endl;
			}
		}
		issueNum=group_issue_num[groupId];
	}
	
	bid->setVal(this->currentValue);
	Myout::out<<"生成一个bid["<<bid->getVal()<<"]"<<endl;
	for (int i=0;i<issueNum;i++)
	{
		Myout::out<<"      :range["<<i<<"]: ["<<bid->getRangeLow(i)<<","<<bid->getRangeUp(i)<<"]"<<endl;
	}
	return currentValue;

}
//退火算法中，对p产生一个扰动，产生一个新的point，changenum是扰动要扰动的议题值，每次扰动随机增加或减小1
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
	Myout::out<<"  [退火算法-测试点生成]根据点："+p->printp()<<"产生扰动后生成点:"<<rp->printp()<<endl;
	return rp;
}
Point *Agent::generiatePoint(const Point * p,int changeNum){
	return Agent::generiatePoint(p,changeNum,ISSUENUM);
}

//for MA，deal identification
Result * Agent::dealBids(Bid ** bidsCa, int num_ca,Bid ** bidsSa,int num_sa){
	Result *result;
	int maxValue=0;
	Result * maxResult=NULL;
	for (int i=0;i<num_ca;i++)
	{
		
		for (int j=0;j<num_sa;j++)
		{
			cout<<"CA第"<<i<<"个BIDs和Sa第"<<j<<"个比较"<<endl;
			Myout::out<<"CA第"<<i<<"个BIDs和Sa第"<<j<<"个比较"<<endl;
			result=bidsCa[i]->getOverlap(bidsSa[j]);
			if (result!=NULL)
			{
				cout<<"成功，产生一个result"<<result->printb()<<endl;
				Myout::out<<"成功，产生一个result"<<result->printb()<<endl;
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
		Myout::out<<"算法2失败"<<endl;
	}
	
	Myout::out<<"算法2结果："<<endl<<maxResult->printb()<<endl;
	return maxResult;
	//
}
