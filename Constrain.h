#ifndef CONSTRAIN_H
#define CONSTRAIN_H
#include "GolbalPameter.h"
#include "Range.h"
#include "Point.h"
#include "Bid.h"
class Constrain{
private:
	//物理意义的第几个议题，如4个议题两个分组，第三个议题在这里beginIssueNum=3，
	int beginIssueNum;//第几个议题开始，
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