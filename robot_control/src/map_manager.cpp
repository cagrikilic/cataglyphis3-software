#include <robot_control/map_manager.h>

MapManager::MapManager()
    : satMap({"sampleProb","hazard"})
{
	woiServ = nh.advertiseService("/control/mapmanager/waypointsofinterest", &MapManager::WOI, this);
	// *** Temporary! Only for Testing ***
	waypoint.x = 87.6;
	waypoint.y = 11.2;
	waypoint.easyProb = 500;
	waypoint.medProb = 200;
	waypoint.hardProb = 44;
	waypoint.terrainHazard = 35;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 100.1;
	waypoint.y = 33.9;
	waypoint.easyProb = 5;
	waypoint.medProb = 2;
	waypoint.hardProb = 9;
	waypoint.terrainHazard = 55;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 150.0;
	waypoint.y = 2.6;
	waypoint.easyProb = 880;
	waypoint.medProb = 800;
	waypoint.hardProb = 4;
	waypoint.terrainHazard = 305;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 77.1;
	waypoint.y = 15.2;
	waypoint.easyProb = 50;
	waypoint.medProb = 600;
	waypoint.hardProb = 84;
	waypoint.terrainHazard = 500;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 0.6;
	waypoint.y = -51.2;
	waypoint.easyProb = 60;
	waypoint.medProb = 20;
	waypoint.hardProb = 440;
	waypoint.terrainHazard = 9;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 97.6;
	waypoint.y = 1.2;
	waypoint.easyProb = 900;
	waypoint.medProb = 20;
	waypoint.hardProb = 4;
	waypoint.terrainHazard = 100;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 200.1;
	waypoint.y = 60.2;
	waypoint.easyProb = 1;
	waypoint.medProb = 21;
	waypoint.hardProb = 7;
	waypoint.terrainHazard = 600;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = 52.2;
	waypoint.y = 19.3;
	waypoint.easyProb = 700;
	waypoint.medProb = 60;
	waypoint.hardProb = 99;
	waypoint.terrainHazard = 10;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = -100.6;
	waypoint.y = -101.2;
	waypoint.easyProb = 80;
	waypoint.medProb = 20;
	waypoint.hardProb = 25;
	waypoint.terrainHazard = 700;
	waypointsOfInterestVector.push_back(waypoint);
	waypoint.x = -87.6;
	waypoint.y = -11.2;
	waypoint.easyProb = 900;
	waypoint.medProb = 40;
	waypoint.hardProb = 44;
	waypoint.terrainHazard = 100;
	waypointsOfInterestVector.push_back(waypoint);
	ROS_INFO("wapointsOfInterestVector Size = %u",waypointsOfInterestVector.size());
	// ***********************************
    satMapPub = nh.advertise<grid_map_msgs::GridMap>("control/mapmanager/satmap",1);
    satMap.setFrameId("satMap");
    satMap.setGeometry(grid_map::Length(300.0, 200.0), 1.0, grid_map::Position(12.0, 65.0));
    satMap.add("sampleProb", 458);
    satMap.add("hazard", 998);
    /*for(grid_map::GridMapIterator it(satMap); !it.isPastEnd(); ++it)
    {
        satMap.at("sampleProb", *it) = 322.9;
        satMap.at("hazard", *it) = 190.1;
    }*/
    satMap.atPosition("sampleProb", grid_map::Position(90.1, 7.0)) = 500.2;
    ROS_INFO("prob at (80,80) = %f", satMap.atPosition("sampleProb", grid_map::Position(80.0, 80.0)));
    ROS_INFO("prob at (90,7.2) = %f", satMap.atPosition("sampleProb", grid_map::Position(90.0, 7.2)));
    ROS_INFO("prob at (90.1,7.0) = %f", satMap.atPosition("sampleProb", grid_map::Position(90.1, 7.0)));
    ROS_INFO("prob at (90.5,7.0) = %f", satMap.atPosition("sampleProb", grid_map::Position(90.5, 7.0)));
    ROS_INFO("prob at (90.5,6.8) = %f", satMap.atPosition("sampleProb", grid_map::Position(90.5, 6.8)));
    grid_map::GridMapRosConverter::toMessage(satMap,satMapMsg);
    ros::Duration(1.0).sleep();
    satMapPub.publish(satMapMsg);
}

bool MapManager::WOI(robot_control::WaypointsOfInterest::Request &req, robot_control::WaypointsOfInterest::Response &res)
{
	res.waypointArray = waypointsOfInterestVector;
	ROS_INFO("sent WOI");
	return true;
}
