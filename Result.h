#ifndef RESULT_H
#define RESULT_H
#include "Range.h"
#include "Bid.h"
#include "GolbalPameter.h"
#include <string>
using namespace std;
class Result{
private:
	//��������ĵڼ������⣬��4�������������飬����������������beginIssueNum=3��
	int beginIssueNum;//�ڼ������⿪ʼ��
	int endIssueNum;
	Range * range[ISSUENUM];
	float val;
	Bid * caBid;
	Bid * saBid;
public:
	Result();
	Result(int be,int en);
	void setRange(float low,float up,int i);
	void setVal(float val);
	float getVal() const;
	string printb() const;
	void setBID(const Bid * caBid,const Bid * saBid);
	~Result();
	
};
#endif