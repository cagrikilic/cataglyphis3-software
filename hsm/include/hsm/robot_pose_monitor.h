#ifndef ROBOT_POSE_MONITOR_H
#define ROBOT_POSE_MONITOR_H
#include <ros/ros.h>
#include <messages/RobotPose.h>
#include <messages/NavFilterOut.h>
#include <messages/SLAMPoseOut.h>

class RobotPoseMonitor
{
public:
	// members
	ros::NodeHandle nh;
	ros::Publisher bestPosePub;
	ros::Subscriber navSub;
	ros::Subscriber slamSub;
	messages::RobotPose bestPoseMsg;
	messages::NavFilterOut navMsg;
	messages::SLAMPoseOut slamMsg;
	ros::Timer poseMonitorTimer;
	const float poseMonitorPeriod = 0.05;
	float navFilterConf;
	float slamConf;
	float northAngle;
	// methods
	RobotPoseMonitor();
	void serviceMonitor(const ros::TimerEvent&);
	void navCallback(const messages::NavFilterOut::ConstPtr& msg);
	void slamCallback(const messages::SLAMPoseOut::ConstPtr& msg);
};

#endif // ROBOT_POSE_MONITOR_H
