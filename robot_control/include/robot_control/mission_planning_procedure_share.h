#ifndef MISSION_PLANNING_PROCESS_SHARE_H
#define MISSION_PLANNING_PROCESS_SHARE_H
#include <vector>
#include <ros/ros.h>
#include <robot_control/Waypoint.h>
#include <robot_control/RegionsOfInterest.h>
#include <robot_control/IntermediateWaypoints.h>
#include <robot_control/ModifyROI.h>
#include <robot_control/SearchMap.h>
#include <robot_control/RandomSearchWaypoints.h>
#include <robot_control/DriveSpeeds.h>
#include <robot_control/cataglyphis_timer.h>
#include <messages/MapPathHazards.h>
#include "robot_status.h"
#include "action_type_enum.h"
#include <messages/ExecAction.h>
#include <messages/ExecInfo.h>
#include <messages/CollisionOut.h>
#include <messages/CVSamplesFound.h>
#include <messages/LidarFilterOut.h>
#include <messages/MasterStatus.h>
#include <messages/NavFilterControl.h>
#include <messages/NextWaypointOut.h>
#include <hsm/voice.h>
#include <armadillo>
#include <math.h>
#include <time.h>
#define PI 3.14159265359
#define DEG2RAD PI/180.0
#define RAD2DEG 180.0/PI
#define NUM_PROC_TYPES 16
#define MAX_SAMPLES 10
#define NUM_TIMERS 5
// !!! If PROC_TYPES_T is ever edited, edit controlCallback_ in MissionPlanning as well
enum PROC_TYPES_T {__initialize__, __emergencyEscape__,__avoid__, __biasRemoval__, __nextBestRegion__, __searchRegion__, __examine__, __approach__, __collect__, __confirmCollect__, __goHome__, __squareUpdate__, __depositApproach__, __depositSample__, __safeMode__, __sosMode__};
enum TIMER_NAMES_T {_roiTimer_, _biasRemovalTimer_, _homingTimer_, _biasRemovalActionTimeoutTimer_, _searchTimer_};

class MissionPlanningProcedureShare
{
public:
	//static std::vector<bool> procsToExecute;
	//static std::vector<bool> procsToInterrupt;
	//static std::vector<bool> procsBeingExecuted;
	static bool procsToExecute[NUM_PROC_TYPES];
	static bool procsToInterrupt[NUM_PROC_TYPES];
	static bool procsBeingExecuted[NUM_PROC_TYPES];
	static bool procsToResume[NUM_PROC_TYPES];
	static unsigned int numProcsBeingOrToBeExecOrRes;
	static unsigned int numProcsBeingOrToBeExec;
    static ros::ServiceClient execActionClient;
    static messages::ExecAction execActionSrv;
	static ros::Subscriber execInfoSub;
	static messages::ExecInfo execInfoMsg;
	static ros::Subscriber lidarFilterSub;
	static messages::LidarFilterOut lidarFilterMsg;
	static ros::Subscriber hsmMasterStatusSub;
	static messages::MasterStatus hsmMasterStatusMsg;
	static ros::Subscriber nextWaypointSub;
	static messages::NextWaypointOut nextWaypointMsg;
    static ros::ServiceClient intermediateWaypointsClient;
    static robot_control::IntermediateWaypoints intermediateWaypointsSrv;
    static ros::ServiceClient reqROIClient;
    static robot_control::RegionsOfInterest regionsOfInterestSrv;
	static ros::ServiceClient modROIClient;
	static robot_control::ModifyROI modROISrv;
	static ros::ServiceClient searchMapClient;
	static robot_control::SearchMap searchMapSrv;
	static ros::ServiceClient randomSearchWaypointsClient;
	static robot_control::RandomSearchWaypoints randomSearchWaypointsSrv;
	static ros::ServiceClient globalMapPathHazardsClient;
	static messages::MapPathHazards globalMapPathHazardsSrv;
	static ros::ServiceClient searchLocalMapPathHazardsClient;
	static messages::MapPathHazards searchLocalMapPathHazardsSrv;
	static ros::ServiceClient navControlClient;
	static messages::NavFilterControl navControlSrv;
	static VoiceBase* voiceSay;
	//#define voiceObj voiceSay;
	static ros::Publisher driveSpeedsPub;
	static robot_control::DriveSpeeds driveSpeedsMsg;
	static robot_control::DriveSpeeds driveSpeedsMsgPrev;
    static RobotStatus robotStatus;
	static CataglyphisTimerBase* timers[NUM_TIMERS];
    static std::vector<robot_control::Waypoint> waypointsToTravel;
    static int numWaypointsToTravel;
    static bool execDequeEmpty;
    static PROC_TYPES_T execLastProcType;
    static unsigned int execLastSerialNum;
	static messages::CollisionOut collisionMsg;
	static float collisionInterruptThresh; // m
	const float collisionMinDistance = 2.0; // m
	static ros::Subscriber cvSamplesSub;
	static messages::CVSamplesFound cvSamplesFoundMsg;
	static messages::CVSampleProps bestSample;
	const float distanceToGrabber = 0.86; // m
	const float distanceToBlindDriveLocation = 0.96; // m
	const float blindDriveDistance = 0.36; // m
	const float sideGrabAngleOffset = 15.0; // deg
	const float initGrabberDistanceTolerance = 0.17; // m
	const float initGrabberAngleTolerance = 6.0; // deg
	const float grabberDistanceToleranceIncrementPerApproachManeuver = 0.01; // m
	const float grabberAngleToleranceIncrementPerApproachManeuver = 0.05; // deg
	const float possibleSampleConfThresh = 0.5;
	const float definiteSampleConfThresh = 0.7;
	static int currentROIIndex;
	static bool initialized;
	static bool escapeCondition;
	static bool performBiasRemoval;
	static bool performHoming;
	static bool inSearchableRegion;
	static bool roiTimeExpired;
	static bool possessingSample;
	static bool possibleSample;
	static bool definiteSample;
	static bool sampleDataActedUpon;
	static bool sampleInCollectPosition;
	static bool sideOffsetGrab;
	static bool confirmedPossession;
	static bool atHome;
	static bool homingUpdateFailed;
	static bool performSafeMode;
	static bool inDepositPosition;
	static bool missionEnded;
	static bool useDeadReckoning;
	static unsigned int samplesCollected;
	static bool avoidLockout;
	static bool escapeLockout;
	static bool roiKeyframed;
	static bool startSLAM;
	static bool giveUpROI;
	static bool newSearchActionOnExec;
	static bool searchTimedOut;
	static bool tiltTooExtremeForBiasRemoval;
	static bool biasRemovalTimedOut;
	static bool navStopRequest;
	static unsigned int avoidCount;
	const unsigned int maxNormalWaypointAvoidCount = 5;
	const unsigned int maxROIWaypointAvoidCount = 8;
	const unsigned int maxCornerWaypointAvoidCount = 12;
	const unsigned int maxHomeWaypointAvoidCount = 20;
	const unsigned int fullyBlockedAvoidCountIncrement = 5;
	const float metersPerAvoidCountDecrement = 3.0;
	static float prevAvoidCountDecXPos;
	static float prevAvoidCountDecYPos;
	static unsigned int numSampleCandidates;
	static std::vector<float> sampleValues;
	static float bestSampleValue;
	static float distanceToDrive; // m
	static float angleToTurn; // deg
	static float expectedSampleDistance;
	static float expectedSampleAngle;
	static messages::CVSampleProps highestConfSample;
	static float allocatedROITime; // sec
	static unsigned int examineCount;
	static unsigned int backUpCount;
	static unsigned int confirmCollectFailedCount;
	static unsigned int homingUpdatedFailedCount;
	static unsigned int navStatus;
	const unsigned int maxHomingUpdatedFailedCount = 3;
	const unsigned int homingFailedSwitchToDeadReckoningCount = 2;
	const float sampleConfidenceGain = 1.0;
	const float sampleDistanceToExpectedGain = 1.0;
	const float defaultVMax = 1.2; // m/s
	const float fastVMax = 1.4; // m/s
	const float slowVMax = 0.6; // m/s
	const float defaultRMax = 45.0; // deg/s
	const float homeWaypointX = 5.0; // m
	const float homeWaypointY = 0.0; // m
	const float lidarUpdateWaitTime = 2.0; // sec
	const float biasRemovalTimeoutPeriod = 180.0; // sec = 3 minutes
	const float homingTimeoutPeriod = 1200.0; // sec = 20 minutes
	const float searchTimeoutPeriod = 15.0; // sec
	const float sampleFoundNewROIProbMultiplier = 0.01;
	const float roiTimeExpiredNewSampleProbMultiplier = 0.05;
	const float giveUpROIFromAvoidNewSampleProbMultiplier = 0.01;
	const float biasRemovalTiltLimit = 5.0; // deg
	const float biasRemovalActionTimeoutTime = 20.0; // sec
	const float shortRecomputeActionWaitTime = 0.1; // sec
};

//std::vector<bool> MissionPlanningProcedureShare::procsToExecute;
//std::vector<bool> MissionPlanningProcedureShare::procsToInterrupt;
//std::vector<bool> MissionPlanningProcedureShare::procsBeingExecuted;
bool MissionPlanningProcedureShare::procsToExecute[NUM_PROC_TYPES];
bool MissionPlanningProcedureShare::procsToInterrupt[NUM_PROC_TYPES];
bool MissionPlanningProcedureShare::procsBeingExecuted[NUM_PROC_TYPES];
bool MissionPlanningProcedureShare::procsToResume[NUM_PROC_TYPES];
unsigned int MissionPlanningProcedureShare::numProcsBeingOrToBeExecOrRes;
unsigned int MissionPlanningProcedureShare::numProcsBeingOrToBeExec;
ros::ServiceClient MissionPlanningProcedureShare::execActionClient;
messages::ExecAction MissionPlanningProcedureShare::execActionSrv;
ros::Subscriber MissionPlanningProcedureShare::execInfoSub;
messages::ExecInfo MissionPlanningProcedureShare::execInfoMsg;
ros::Subscriber MissionPlanningProcedureShare::lidarFilterSub;
messages::LidarFilterOut MissionPlanningProcedureShare::lidarFilterMsg;
ros::Subscriber MissionPlanningProcedureShare::hsmMasterStatusSub;
messages::MasterStatus MissionPlanningProcedureShare::hsmMasterStatusMsg;
ros::Subscriber MissionPlanningProcedureShare::nextWaypointSub;
messages::NextWaypointOut MissionPlanningProcedureShare::nextWaypointMsg;
ros::ServiceClient MissionPlanningProcedureShare::intermediateWaypointsClient;
robot_control::IntermediateWaypoints MissionPlanningProcedureShare::intermediateWaypointsSrv;
ros::ServiceClient MissionPlanningProcedureShare::reqROIClient;
robot_control::RegionsOfInterest MissionPlanningProcedureShare::regionsOfInterestSrv;
ros::ServiceClient MissionPlanningProcedureShare::modROIClient;
robot_control::ModifyROI MissionPlanningProcedureShare::modROISrv;
ros::ServiceClient MissionPlanningProcedureShare::searchMapClient;
robot_control::SearchMap MissionPlanningProcedureShare::searchMapSrv;
ros::ServiceClient MissionPlanningProcedureShare::randomSearchWaypointsClient;
robot_control::RandomSearchWaypoints MissionPlanningProcedureShare::randomSearchWaypointsSrv;
ros::ServiceClient MissionPlanningProcedureShare::globalMapPathHazardsClient;
messages::MapPathHazards MissionPlanningProcedureShare::globalMapPathHazardsSrv;
ros::ServiceClient MissionPlanningProcedureShare::searchLocalMapPathHazardsClient;
messages::MapPathHazards MissionPlanningProcedureShare::searchLocalMapPathHazardsSrv;
VoiceBase* MissionPlanningProcedureShare::voiceSay;
ros::ServiceClient MissionPlanningProcedureShare::navControlClient;
messages::NavFilterControl MissionPlanningProcedureShare::navControlSrv;
ros::Publisher MissionPlanningProcedureShare::driveSpeedsPub;
robot_control::DriveSpeeds MissionPlanningProcedureShare::driveSpeedsMsg;
robot_control::DriveSpeeds MissionPlanningProcedureShare::driveSpeedsMsgPrev;
RobotStatus MissionPlanningProcedureShare::robotStatus;
CataglyphisTimerBase* MissionPlanningProcedureShare::timers[NUM_TIMERS];
std::vector<robot_control::Waypoint> MissionPlanningProcedureShare::waypointsToTravel;
int MissionPlanningProcedureShare::numWaypointsToTravel;
bool MissionPlanningProcedureShare::execDequeEmpty;
PROC_TYPES_T MissionPlanningProcedureShare::execLastProcType;
unsigned int MissionPlanningProcedureShare::execLastSerialNum;
messages::CollisionOut MissionPlanningProcedureShare::collisionMsg;
float MissionPlanningProcedureShare::collisionInterruptThresh;
ros::Subscriber MissionPlanningProcedureShare::cvSamplesSub;
messages::CVSamplesFound MissionPlanningProcedureShare::cvSamplesFoundMsg;
messages::CVSampleProps MissionPlanningProcedureShare::bestSample;
int MissionPlanningProcedureShare::currentROIIndex;
bool MissionPlanningProcedureShare::initialized;
bool MissionPlanningProcedureShare::escapeCondition;
bool MissionPlanningProcedureShare::performBiasRemoval;
bool MissionPlanningProcedureShare::performHoming;
bool MissionPlanningProcedureShare::inSearchableRegion;
bool MissionPlanningProcedureShare::roiTimeExpired;
bool MissionPlanningProcedureShare::possessingSample;
bool MissionPlanningProcedureShare::possibleSample;
bool MissionPlanningProcedureShare::definiteSample;
bool MissionPlanningProcedureShare::sampleDataActedUpon;
bool MissionPlanningProcedureShare::sampleInCollectPosition;
bool MissionPlanningProcedureShare::sideOffsetGrab;
bool MissionPlanningProcedureShare::confirmedPossession;
bool MissionPlanningProcedureShare::atHome;
bool MissionPlanningProcedureShare::homingUpdateFailed;
bool MissionPlanningProcedureShare::performSafeMode;
bool MissionPlanningProcedureShare::inDepositPosition;
bool MissionPlanningProcedureShare::missionEnded;
bool MissionPlanningProcedureShare::useDeadReckoning;
unsigned int MissionPlanningProcedureShare::samplesCollected;
bool MissionPlanningProcedureShare::avoidLockout;
bool MissionPlanningProcedureShare::escapeLockout;
bool MissionPlanningProcedureShare::roiKeyframed;
bool MissionPlanningProcedureShare::startSLAM;
bool MissionPlanningProcedureShare::giveUpROI;
bool MissionPlanningProcedureShare::tiltTooExtremeForBiasRemoval;
bool MissionPlanningProcedureShare::searchTimedOut;
bool MissionPlanningProcedureShare::newSearchActionOnExec;
bool MissionPlanningProcedureShare::biasRemovalTimedOut;
bool MissionPlanningProcedureShare::navStopRequest;
unsigned int MissionPlanningProcedureShare::avoidCount;
float MissionPlanningProcedureShare::prevAvoidCountDecXPos;
float MissionPlanningProcedureShare::prevAvoidCountDecYPos;
unsigned int MissionPlanningProcedureShare::numSampleCandidates;
std::vector<float> MissionPlanningProcedureShare::sampleValues;
float MissionPlanningProcedureShare::bestSampleValue;
float MissionPlanningProcedureShare::distanceToDrive;
float MissionPlanningProcedureShare::angleToTurn;
float MissionPlanningProcedureShare::expectedSampleDistance;
float MissionPlanningProcedureShare::expectedSampleAngle;
messages::CVSampleProps MissionPlanningProcedureShare::highestConfSample;
float MissionPlanningProcedureShare::allocatedROITime;
unsigned int MissionPlanningProcedureShare::examineCount;
unsigned int MissionPlanningProcedureShare::backUpCount;
unsigned int MissionPlanningProcedureShare::confirmCollectFailedCount;
unsigned int MissionPlanningProcedureShare::homingUpdatedFailedCount;
unsigned int MissionPlanningProcedureShare::navStatus;

#endif // MISSION_PLANNING_PROCESS_SHARE_H
