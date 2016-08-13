#ifndef ROS_WORKERS
#define ROS_WORKERS

#include <QWidget>
#include <QThread>

#include <boost/shared_ptr.hpp>
#include <QMutex>
#include <ros/ros.h>

//#include <grid_map_ros/grid_map_ros.hpp>
//#include <grid_map_msgs/GridMap.h>

#include <messages/NavFilterOut.h>
#include <messages/NavFilterControl.h>

#include <messages/HSMSetNorthAngle.h>
#include <messages/RobotPose.h>
#include <messages/GlobalMapFull.h>

#include <robot_control/RegionsOfInterest.h>
#include <robot_control/ROI.h>

#include <map_viewer_enums.h>

#define ON_SERIVCE_FAILURE_RETURN_PAUSE 3
#define NAV_INFO_MIN_PUB_TIME 1.00
#define HSM_POSE_MIN_PUB_TIME 1.00

class ros_workers : public QObject
{
    Q_OBJECT

signals:
    void nav_service_returned(const messages::NavFilterControl navResponse,
                                  bool wasSucessful,
                                    const int callerID);

    void nav_init_returned(const messages::NavFilterControl navResponse,
                                  bool wasSucessful);

    void bias_removal_returned(const messages::NavFilterControl navResponse,
                                           bool wasSucessful);

    void dead_reckoning_service_returned(const messages::NavFilterControl navResponse,
                                            bool wasSucessful);

    void nav_info_callback(const messages::NavFilterOut navInfo);

    void hsm_global_pose_callback(const messages::RobotPose hsmRobotPose);

    void map_manager_ROI_service_returned(const robot_control::RegionsOfInterest mapManagerResponse,
                                            bool wasSucessful);
    void map_manager_global_map_service_returned(const messages::GlobalMapFull &gridMapFull, map_viewer_enums::mapViewerLayers_t requestedLayer,
                                                    bool wasSucessful);

public slots:
    void on_run_nav_service(messages::NavFilterControl serviceRequest, const int callerID);

    void on_run_bias_removal_service();
    void on_run_start_dead_reckoning_service();
    void on_run_nav_init_service(messages::NavFilterControl serviceRequest);

    void on_run_map_manager_ROI_service();
    void on_run_map_manager_global_map_request(map_viewer_enums::mapViewerLayers_t requestedLayer);

    void on_run_nav_info_subscriber_start();
    void on_run_nav_info_subscriber_stop();

    void on_run_hsm_global_pose_subscriber_start();
    void on_run_hsm_global_pose_subscriber_stop();

private:
    boost::shared_ptr<ros::NodeHandle> nh;
    ros::ServiceClient navControlClient;

    ros::ServiceClient hsmNAControlClient;
    messages::HSMSetNorthAngle lastHSMNAMsg;

    ros::ServiceClient mapManagerROIClient;
    robot_control::RegionsOfInterest lastROIMsg;

    ros::ServiceClient mapManagerGlobalMapClient;
    messages::GlobalMapFull lastGlobalMapMsg;

    ros::Time navInfoTime;
    bool navInfoSubStarted;
    ros::Subscriber navInfoSub;
    messages::NavFilterOut lastNavMsg;
    void getNavInfoCallback(const messages::NavFilterOut::ConstPtr &msg);

    ros::Time hsmGlobalPoseTime;
    bool hsmGlobalPoseSubStarted;
    ros::Subscriber hsmGlobalPosSub;
    messages::RobotPose lastHSMGlobalPoseMsg;
    void getHSMGlobalPoseCallback(const messages::RobotPose::ConstPtr &msg);

public:
    ros_workers(boost::shared_ptr<ros::NodeHandle> nhArg);

};

#endif // ROS_WORKERS

