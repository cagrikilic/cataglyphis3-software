#ifndef DRIVE_PIVOT_H
#define DRIVE_PIVOT_H
#include "task.h"

class DrivePivot : public Task
{
public:
	void init();
	int run();
private:
	float desiredDeltaHeading_;
	float deltaHeading_;
	float rMax_;
	float initHeading_;
	float rDes_;
	float errorR_;
	int pivotSign_;
	int leftSpeed_;
	int rightSpeed_;
	int rSpeedT_;
	float rSpeedP_;
	float rSpeedI_;
	unsigned int timeoutValue_;
	unsigned int timeoutCounter_;
	double thresholdInitTime_;
	double thresholdTime_;
	bool inThreshold_;
	int taskEnded_;
	const float kpR_ = 0.85; // deg/(s*deg)
	const float kiR_ = 0.25;
	const float rSpeedIMax_ = 100.0;
	const float kROutput_ = 450/45.0; // 45% of max speed at 45 deg/s
	const int rSpeedMax_ = 500;
	const float deltaHeadingThreshold_ = 2.0; // deg
	const double thresholdMinTime_ = 0.25; // s
	const float middleWheelReduction_ = 0.65;
};

#endif // DRIVE_PIVOT_H
