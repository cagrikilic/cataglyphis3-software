#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H
#include <ros/ros.h>
#include <robot_control/Waypoint.h>
#include <robot_control/RegionsOfInterest.h>
#include <robot_control/ModifyROI.h>
#include <robot_control/CurrentROI.h>
#include <messages/ROIGridMap.h>
#include <messages/KeyframeList.h>
#include <messages/RobotPose.h>
#include <grid_map_ros/grid_map_ros.hpp>
#include <grid_map_msgs/GridMap.h>
#include "global_map_layers.h"
#include <vector>

#define PI 3.14159265359
#define DEG2RAD PI/180.0
#define RAD2DEG 180.0/PI

class MapManager
{
public:
	// Methods
	MapManager(); // Constructor
	bool listROI(robot_control::RegionsOfInterest::Request &req, robot_control::RegionsOfInterest::Response &res);
	bool modROI(robot_control::ModifyROI::Request &req, robot_control::ModifyROI::Response &res);
	bool mapROI(messages::ROIGridMap::Request &req, messages::ROIGridMap::Response &res);
	void keyframesCallback(const messages::KeyframeList::ConstPtr& msg);
	void robotPoseCallback(const messages::RobotPose::ConstPtr& msg);
	void clearGlobalMapLayers(int startIndex, int endIndex);
	// Members
	ros::NodeHandle nh;
	ros::ServiceServer roiServ;
	ros::ServiceServer modROIServ;
	ros::ServiceServer mapROIServ;
	ros::Subscriber keyframesSub;
	ros::Subscriber robotPoseSub;
	ros::Publisher currentROIPub;
	robot_control::Waypoint waypoint;
	robot_control::ROI ROI;
	std::vector<robot_control::ROI> regionsOfInterest;
	grid_map::GridMap globalMap;
	ros::Publisher globalMapPub;
	grid_map_msgs::GridMap globalMapMsg;
	messages::RobotPose robotPose;
	messages::KeyframeList keyframes;
	grid_map::GridMap currentKeyframe;
	grid_map::GridMap keyframeTransform; // ***
	float keyframeOriginalXLen; // ***
	float keyframeOriginalYLen; // ***
	float keyframeXPos;
	float keyframeYPos;
	grid_map::Position keyframeCoord;
	//float keyframeOriginalYCoord;
	float keyframeTransformXLen; // ***
	float keyframeTransformYLen; // ***
	float keyframeHeading;
	grid_map::Position globalXTransformCoord;
	//float globalYTransformPos;
	robot_control::CurrentROI currentROIMsg;
	const float mapResolution = 1.0; // m
};

#endif // MAP_MANAGER_H
