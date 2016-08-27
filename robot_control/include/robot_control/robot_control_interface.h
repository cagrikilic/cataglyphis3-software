#ifndef ROBOT_CONTROL_INTERFACE_H
#define ROBOT_CONTROL_INTERFACE_H
#include "robot_outputs.h"
#include "robot_status.h"
#include <messages/CVSearchCmd.h>
#include <ros/ros.h>
#include "bit_utils.h"

class RobotControlInterface
{
public:
    static RobotStatus robotStatus;
    static RobotOutputs robotOutputs;
	static ros::ServiceClient cvSearchCmdClient;
	static messages::CVSearchCmd cvSearchCmdSrv;
	static Leading_Edge_Latch dropStatusLEL_;
	static Leading_Edge_Latch slideStatusLEL_;
};

RobotStatus RobotControlInterface::robotStatus;
RobotOutputs RobotControlInterface::robotOutputs;
ros::ServiceClient RobotControlInterface::cvSearchCmdClient;
messages::CVSearchCmd RobotControlInterface::cvSearchCmdSrv;
Leading_Edge_Latch RobotControlInterface::dropStatusLEL_;
Leading_Edge_Latch RobotControlInterface::slideStatusLEL_;

#endif // ROBOT_CONTROL_INTERFACE_H
