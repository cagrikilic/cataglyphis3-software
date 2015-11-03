#ifndef ROBOT_STATUS_H
#define ROBOT_STATUS_H

class RobotStatus
{
public:
	unsigned int loopRate = 20; // Hz. Default to 20 Hz
	// Drive data
	float xPos = 0.0; // m
	float yPos = 0.0; // m
	float heading = 0.0; // deg
	float bearing = 0.0; // deg
	float distToOrigin = 0.0; // m
	float yawRate = 0.0; // deg/s
	// Grabber data
	int grabberSlideStatus = 0;
	int grabberDropStatus = 0;
	int grabberSlidePos = 0;
	int grabberDropPos = 0;
	// Vision data
	float panAngle = 0.0; // deg
	float neckPos = 0.0; // deg
};

#endif // ROBOT_STATUS_H
