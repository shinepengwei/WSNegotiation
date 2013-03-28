#ifndef RANGE_H
#define RANGE_H
class Range{
	friend class Constrain;
	friend class Bid;
	friend class Result;
private:
	float low;
	float up;
public:

	void setNum(const float,const float);
	Range(float,float);
	float getLow();
	float getUp();
};
#endif