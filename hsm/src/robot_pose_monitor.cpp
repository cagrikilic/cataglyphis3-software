#include <robot_pose_monitor.h>

RobotPoseMonitor::RobotPoseMonitor()
{
	bestPosePub = nh.advertise<messages::RobotPose>("/hsm/masterexec/globalpose", 1);
	navSub = nh.subscribe<messages::NavFilterOut>("navigation/navigationfilterout/navigationfilterout", 1, &RobotPoseMonitor::navCallback, this);
	slamSub = nh.subscribe<messages::SLAMPoseOut>("/slam/localizationnode/slamposeout", 1, &RobotPoseMonitor::slamCallback, this);
	poseMonitorTimer = nh.createTimer(ros::Duration(poseMonitorPeriod), &RobotPoseMonitor::serviceMonitor, this);
	// Trust dead reckoning right at beginning before any keyframes have been generated
	navFilterConf = 1.0;
	slamConf = 0.9;
    navMsg.north_angle = 90.0;
    divergenceDetected = false;
    navSolutionsDiverged = false;
    divergenceState = __notDiverged__;
}

void RobotPoseMonitor::serviceMonitor(const ros::TimerEvent&)
{
	// compute confidence in nav filter solution
	navFilterConf = 1.0;
	// compute confidence in slam solution
	slamConf = 0.0;
	// compare confidences in soltions and choose the one with the higher confidence
	if(slamConf > navFilterConf)
	{
		bestPoseMsg.x = slamMsg.globalX;
		bestPoseMsg.y = slamMsg.globalY;
		bestPoseMsg.heading = slamMsg.globalHeading;
		bestPoseMsg.humanHeading = fmod(bestPoseMsg.heading, 360.0);
        bestPoseMsg.northAngle = navMsg.north_angle;
		bestPoseMsg.homingUpdated = navMsg.homing_updated;
        bestPoseMsg.platformNumber = navMsg.platform_number;
	}
	else
	{
		bestPoseMsg.x = navMsg.x_position;
		bestPoseMsg.y = navMsg.y_position;
		bestPoseMsg.heading = navMsg.heading;
		bestPoseMsg.humanHeading = fmod(bestPoseMsg.heading, 360.0);
        bestPoseMsg.northAngle = navMsg.north_angle;
		bestPoseMsg.homingUpdated = navMsg.homing_updated;
        bestPoseMsg.platformNumber = navMsg.platform_number;
	}
	navSolutionsDiverged = false;
    /*navSolutionError = hypot(slamMsg.globalX - navMsg.x_position, slamMsg.globalY - navMsg.y_position);
    switch(divergenceState)
    {
    case __notDiverged__:
        if(navSolutionError > divergenceTriggerDistance && !divergenceDetected)
        {
            divergenceDetected = true;
            divergenceStartTime = ros::Time::now().toSec();
        }
        else if(navSolutionError <= divergenceTriggerDistance && divergenceDetected) divergenceDetected = false;
        if(((ros::Time::now().toSec() - divergenceStartTime) > divergenceTriggerTime) && divergenceDetected) divergenceState = __diverged__;
        else divergenceState = __notDiverged__;
        navSolutionsDiverged = false;
        break;
    case __diverged__:
        if(navSolutionError <= divergenceTriggerDistance) divergenceState = __notDiverged__;
        else divergenceState = __diverged__;
        navSolutionsDiverged = true;
        break;
    }*/
	bestPosePub.publish(bestPoseMsg);
}

void RobotPoseMonitor::navCallback(const messages::NavFilterOut::ConstPtr &msg)
{
	navMsg = *msg;
}

void RobotPoseMonitor::slamCallback(const messages::SLAMPoseOut::ConstPtr &msg)
{
	slamMsg = *msg;
}
