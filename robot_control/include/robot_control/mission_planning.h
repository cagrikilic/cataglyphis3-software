#ifndef MISSION_PLANNING_H
#define MISSION_PLANNING_H
#include <ros/ros.h>
#include "mission_planning_procedure_share.h"
#include <messages/RobotPose.h>
#include <messages/ExecActionEnded.h>
#include <messages/nb1_to_i7_msg.h>
#include <messages/EmergencyEscapeTrigger.h>
#include <messages/MissionPlanningInfo.h>
#include <messages/MissionPlanningControl.h>
#include <messages/NavFilterOut.h>
#include "emergency_escape.h"
#include "avoid.h"
#include "next_best_region.h"
#include "search_region.h"
#include "examine.h"
#include "approach.h"
#include "collect.h"
#include "confirm_collect.h"
#include "go_home.h"
#include "deposit_approach.h"
#include "deposit_sample.h"
#include "pause.h"
#include "bit_utils.h"

class MissionPlanning : public MissionPlanningProcedureShare
{
public:
	// Methods
	MissionPlanning();
	void run();
	// Members
	ros::NodeHandle nh;
	ros::Publisher infoPub;
	ros::Subscriber poseSub;
	ros::Subscriber ExecActionEndedSub;
    ros::Subscriber nb1Sub;
	ros::Subscriber collisionSub;
	ros::Subscriber navSub;
	ros::ServiceServer emergencyEscapeServ;
	ros::ServiceServer controlServ;
	messages::MissionPlanningInfo infoMsg;
    messages::nb1_to_i7_msg nb1Msg;
	const int loopRate = 20; // Hz
	EmergencyEscape emergencyEscape;
	Avoid avoid;
	NextBestRegion nextBestRegion;
	SearchRegion searchRegion;
	Examine examine;
	Approach approach;
	Collect collect;
	ConfirmCollect confirmCollect;
	GoHome goHome;
	DepositApproach depositApproach;
	DepositSample depositSample;
    Pause pause;

	bool collisionInterruptTrigger;
	Leading_Edge_Latch collisionInterruptLEL;
	unsigned int numProcsBeingOrToBeExec;
	bool multiProcLockout;
	unsigned int lockoutSum;
	bool initComplete;
	bool pauseStarted;
	const float homeX = 5.0; // m
	const float homeY = 0.0; // m
	float avoidRemainingWaypointDistance;
	const float minAvoidRemainingWaypointDistance = 2.0; // m
	bool shouldExecuteAvoidManeuver;
private:
	void evalConditions_();
	void runProcesses_();
	void runPause_();
	void pauseAllTimers_();
	void resumeTimers_();
	void calcnumProcsBeingOrToBeExec_();
	void updateSampleFlags_();
	void packAndPubInfoMsg_();
	void poseCallback_(const messages::RobotPose::ConstPtr& msg);
	void ExecActionEndedCallback_(const messages::ExecActionEnded::ConstPtr& msg);
    void nb1Callback_(const messages::nb1_to_i7_msg::ConstPtr& msg);
	void collisionCallback_(const messages::CollisionOut::ConstPtr& msg);
	void navCallback_(const messages::NavFilterOut::ConstPtr& msg);
	void execInfoCallback_(const messages::ExecInfo::ConstPtr& msg);
	void cvSamplesCallback_(const messages::CVSamplesFound::ConstPtr& msg);
	void lidarFilterCallback_(const messages::LidarFilterOut::ConstPtr& msg);
	void hsmMasterStatusCallback_(const messages::MasterStatus::ConstPtr& msg);
	bool emergencyEscapeCallback_(messages::EmergencyEscapeTrigger::Request &req, messages::EmergencyEscapeTrigger::Response &res);
	bool controlCallback_(messages::MissionPlanningControl::Request &req, messages::MissionPlanningControl::Response &res);
	void biasRemovalTimerCallback_(const ros::TimerEvent& event);
	void homingTimerCallback_(const ros::TimerEvent& event);
};

#endif // MISSION_PLANNING_H
