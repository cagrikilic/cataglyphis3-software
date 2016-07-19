#ifndef WAIT_H
#define WAIT_H
#include "action.h"

class Wait : public Action
{
public:
	Wait(); // constructor
	void init();
	int run();
private:
	ros::NodeHandle nh;
	float waitTime_; // sec
	ros::Timer timer;
	bool timeExpired_;
	void waitTimeCallback_(const ros::TimerEvent &event);
};

#endif // WAIT_H
