#ifndef AGENT_H
#define AGENT_H
#include <string>

#include "Constrain.h"
#include "Point.h"
#include "Bid.h"
#include "Result.h"
#include "GolbalPameter.h"
using namespace std;
class Agent{
private:
	Constrain * cs[GROUP_NUM][MAX_CS_NUM];
	int group[GROUP_NUM][2];
	float currentValue;//在退火算法中使用，当前接受值
	string name;
public:
	Agent(string );
	float accept(const Point * p,float);
	float accept(const Point *p,float,int);
	int getBids(Bid ** bids);
	int getBids(Bid ** bids,int groupId);
	static Result * dealBids(Bid ** bidsCa, int num_ca,Bid ** bidsSa,int num_sa);//for MA，deal identification
	static Point * generiatePoint(const Point * p,int changeNum,int issueNum);
	static Point * generiatePoint(const Point * p,int changeNum);

private:
	float generiateBids(Bid *,const Point * p,int);
	float getSumSatifyValue(const Point *p,int groupId);
};
#endif