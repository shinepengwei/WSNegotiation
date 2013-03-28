#ifndef CONSTRAIN_H
#define CONSTRAIN_H
#include "GolbalPameter.h"
#include "Range.h"
#include "Point.h"
#include "Bid.h"
class Constrain{
private:
	//��������ĵڼ������⣬��4�������������飬����������������beginIssueNum=3��
	int beginIssueNum;//�ڼ������⿪ʼ��
	int endIssueNum;
	Range * range[MAXGROUPISSUENUM];
	float val;
	void genernateRandom(int);
public:
	Constrain(int begin_num,int end_num,int,string);
	~Constrain();
	void setRange(float range_low,float range_up,int issue_num);
	float getVal() const;
	float getRangeLow(int i) const;
	float getRangeUp(int) const;
	bool satify(const Point *p,bool isGruop);
	void cutBids(Bid *,bool isGruop);
	string printc() const;
};
#endif