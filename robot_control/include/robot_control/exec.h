#ifndef EXEC_H
#define EXEC_H
#include <ros/ros.h>
#include <deque>
#include "action_type_enum.h"
#include "action.h"
#include "action_params.h"
#include "idle.h"
#include "halt.h"
#include "drive_global.h"
#include "drive_relative.h"
#include "drop.h"
#include "grab.h"
#include "open.h"
#include <messages/ExecAction.h>
#include <messages/ActuatorOut.h>
#include <messages/ExecInfo.h>
#include <messages/NavFilterOut.h>
#include <messages/GrabberFeedback.h>

#define ACTION_POOL_SIZE 10

class Exec : public RobotControlInterface
{
public:
	// Members
	ros::NodeHandle nh;
	ros::Publisher infoPub;
	ros::Publisher actuatorPub;
	ros::Subscriber actionSub;
	ros::Subscriber navSub;
	ros::Subscriber grabberSub;
	const int loopRate = 20; // Hz
	// Methods
	Exec(); // Constructor
	void run(); // Main run method for exec
private:
	// Members
	std::deque <Action*> actionDeque_;
	Idle pauseIdle_;
	ACTION_TYPE_T nextActionType_ = _idle;
	bool newActionFlag_ = false;
	bool pushToFrontFlag_ = false;
	bool clearDequeFlag_ = false;
	bool pause_ = false;
	bool pausePrev_ = false;
	bool actionDequeEmpty_ = false;
	int currentActionDone_ = 0;
	size_t actionDequeSize_ = 0;
	unsigned int actionPoolIndex_[NUM_ACTIONS];
	Action* actionPool_[NUM_ACTIONS][ACTION_POOL_SIZE];
	ACTION_PARAMS_T params_;
	messages::ActuatorOut actuatorMsgOut_;
	messages::ExecInfo execInfoMsgOut_;
	// Methods
	void actionCallback_(const messages::ExecAction::ConstPtr& msg);
	void navCallback_(const messages::NavFilterOut::ConstPtr& msg);
	void grabberCallback_(const messages::GrabberFeedback::ConstPtr& msg);
	void packActuatorMsgOut_();
	void packInfoMsgOut_();
};

#endif // EXEC_H
