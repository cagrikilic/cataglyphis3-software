#ifndef DRIVE_RELATIVE_H
#define DRIVE_RELATIVE_H
#include "action.h"

class DriveRelative : public Action
{
public:
	void init();
	int run();
private:
	float distanceToDrive_;
	float angleToTurn_;
	float vMax_;
	float rMax_;
	float desiredEndHeading_;
	bool endHeading_;
	float candidateEndHeadingAngleToTurn_[2];
	bool pushedToFront_;
	bool closedLoop_;
};

#endif // DRIVE_RELATIVE_H
