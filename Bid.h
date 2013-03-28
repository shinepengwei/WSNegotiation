#ifndef BID_H
#define BID_H
#include "Range.h"
#include "GolbalPameter.h"
#include <string>
using namespace std;
class Bid{
private:
	//物理意义的第几个议题，如4个议题两个分组，第三个议题在这里beginIssueNum=3，
	int beginIssueNum;//第几个议题开始，
	int endIssueNum;
	Range * range[ISSUENUM];
	float val;
public:
	Bid();
	Bid(const Bid * b);
	Bid(int be,int en);
	void setRange(float low,float up,int i);
	void setVal(float val);
	float getRangeLow(int i) const;
	float getRangeUp(int i) const;
	float getVal() const;
	int getNum() const;
	string printb() const;
	bool compareTo(Bid *) const;
	Result * getOverlap(const Bid * b)const;
	~Bid();
	
};
#endif