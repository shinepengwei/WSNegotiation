#include <iostream>
#include <stdio.h> 
#include <math.h>
#include <string>
#include "MyOut.h"

#include "Constrain.h"
#include "Point.h"
#include "Agent.h"
using namespace std;

 
void test_Agent_Stisfy(){
	Agent a("test");
	Point p(ISSUENUM);
	p.setValue(8,0);
	p.setValue(6,1);
	p.setValue(4,2);
	p.setValue(5,3);
	a.accept(&p,50);
	p.setValue(2,0);
	p.setValue(6,1);
	p.setValue(5,2);
	p.setValue(7,3);
	a.accept(&p,50);
	p.setValue(2,0);
	p.setValue(4,1);
	p.setValue(5,2);
	p.setValue(7,3);
	a.accept(&p,50);
	p.setValue(2,0);
	p.setValue(6,1);
	p.setValue(6,2);
	p.setValue(7,3);
	a.accept(&p,50);
}
void test_Agent_Stisfy_Group(){
	Agent a("test");
	Point p(2);
	p.setValue(8,0);
	p.setValue(6,1);
	a.accept(&p,50,1);

	p.setValue(2,0);
	p.setValue(6,1);
	a.accept(&p,50,1);

	p.setValue(2,0);
	p.setValue(4,1);
	a.accept(&p,50,1);

	p.setValue(3,0);
	p.setValue(7,1);
	a.accept(&p,50,1);
}

void testAgentGeneriatePoint(){
	Point *p=new Point(ISSUENUM);
	p->setValue(5,0);
	p->setValue(5,1);
	p->setValue(5,2);
	p->setValue(5,3);
	Point * newp=Agent::generiatePoint(p,3);
	Myout::out<<newp->getValue(0)<<","<<newp->getValue(1)<<","<<newp->getValue(2)<<","<<newp->getValue(3)<<","<<endl;

}
void testAgentGetBids(){
	Agent c("ca");
	Bid * bids[2*ISSUENUM];
	c.getBids(bids);
}
void testAgentGetBids_Group(){
	Agent c("ca");
	Bid * bids[200*ISSUENUM];
	c.getBids(bids,1);
}


void negotiation()
{
	//�㷨һ
	int number=5000;
	float T=50;//��ʼ�¶�
	float discount=(float)50/number;//�¶��ۿ�
	Agent ca("CA");
	Agent sa("SA");
	//����һ����
	Point *p=new Point(ISSUENUM);
	p->setValue(5,0);
	p->setValue(5,1);
	p->setValue(5,2);
	p->setValue(5,3);
	Point *newp=new Point(ISSUENUM);
	newp->setValue(5,0);
	newp->setValue(5,1);
	newp->setValue(5,2);
	newp->setValue(5,3);
	float max=0;
	float cur=0;
	for (int i=0;i<number;i++)
	{
		float c=ca.accept(newp,T);
		float s=sa.accept(newp,T);
		if (c>0&&s>0)
		{//ɾ��p����newp��ֵ��p������ͨ��һ��С���Ŷ�����һ���µ�newp
			delete p;
			p=newp;
			newp=Agent::generiatePoint(p,1);
			cur=c+s;
			if (cur>max)
			{
				max=cur;
			}
			Myout::out<<"["<<i<<"]����:"<<cur<<endl;
		}
		else{
			//ɾ��newp��ͨ��һ������Ŷ�����һ���µ�newp��
			delete newp;
			newp=Agent::generiatePoint(p,ISSUENUM);
			Myout::out<<"["<<i<<"]�ܾ�"<<endl;
		}
		T=T-discount;
		cout<<"�˻��㷨"<<i<<endl;
	}
	Myout::out<<"T:"<<T<<endl;
	Myout::out<<cur<<endl;
	Myout::out<<max<<endl;
	//�㷨2��

	Bid * cabids[200*ISSUENUM];
	int caBidNum=ca.getBids(cabids);
	Bid * sabids[200*ISSUENUM];
	int saBidNum=sa.getBids(sabids);
	Result *res;
	res=Agent::dealBids(cabids,caBidNum,sabids,saBidNum);

	Result *maxRes[GROUP_NUM];
	for (i=0;i<GROUP_NUM;i++)
	{
		Bid * cabids[200*ISSUENUM];
		int caBidNum=ca.getBids(cabids,i);
		
		
		Bid * sabids[200*ISSUENUM];
		int saBidNum=sa.getBids(sabids,i);
		maxRes[i]=Agent::dealBids(cabids,caBidNum,sabids,saBidNum);
	}

	Myout::out<<"�㷨2���ս����"<<endl;
	Myout::out<<res->printb()<<endl;

	Myout::out<<"�㷨3���ս����"<<endl;
	for (i=0;i<GROUP_NUM;i++)
		Myout::out<<maxRes[i]->printb()<<endl;

	Myout::out<<"�㷨һ"<<endl;
	Myout::out<<cur<<endl;
	Myout::out<<max<<endl;


}
void testAlgorith2(){
	Agent ca("ca");
	Bid * cabids[2*ISSUENUM];
	int caBidNum=ca.getBids(cabids);

	Agent sa("sa");
	Bid * sabids[2*ISSUENUM];
	int saBidNum=sa.getBids(sabids);
	Result *res=	Agent::dealBids(cabids,caBidNum,sabids,saBidNum);
}
void testAlgorith3(){
	Agent ca("ca");
	Agent sa("sa");
	
	
	
}
void test(){
	//test_Agent_Stisfy();
	//testAgentGeneriatePoint();
	//testAgentGetBids();
//	testAlgorith2();
	//test_Agent_Stisfy_Group();
	//testAgentGetBids_Group();
	testAlgorith3();
	
}
void main()
{
	Myout::out.clear();
	//test();
	negotiation();

	
	string s="end";
	cout<<s<<endl;
	char c;
	cin>>c;
}
