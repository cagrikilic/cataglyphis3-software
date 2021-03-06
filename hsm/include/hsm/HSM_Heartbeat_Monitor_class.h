#ifndef HSM_HEARTBEAT_MONITOR_CLASS_H
#define HSM_HEARTBEAT_MONITOR_CLASS_H

#include <ros/ros.h>
#include <hsm/HSM_Detection.h>
#include <hsm/HSM_Action.h>
#include <string>
#include "Counter.h"
#include "monitor_enums.h"

#define NODE_STARTUP_TIMEOUT 30

class HSM_Heartbeat_Monitor_class
{
private:
	//Members
	std::string hb_node_name;
	int hb_node_Hz;
	ros::Duration hb_timeout_period;
	bool hb_node_died;
	ros::Timer heartbeat_timer;
	
	//Methods

public:
	//Members
	//--INPUTS: heartbeats
	ros::Subscriber sub_heartbeats;
	hsm::HSM_Detection heartbeat_data; //local copy of detection msg data

	//--OUTPUTS: node restart actions
	ros::Publisher pub_action;
	hsm::HSM_Action msg_out;

	//--monitor state machine variables
	monitor_states_t monitor_state;
	recovering_substates_t recovering_substate;
	std::string recovering_error_type;
	ros::Timer recovery_timer; 
	std::string system_string;
	int monitor_trigger_count = 0;
	const int monitor_trigger_limit = 1;
	
	//Methods
	HSM_Heartbeat_Monitor_class(std::string hb_node_name, int hb_node_Hz); //constructor
	void service_monitor();
	void detectionCallback_heartbeat(const hsm::HSM_Detection::ConstPtr& msg_in);
	void recoveringTimerCallback(const ros::TimerEvent& event);
	void heartbeatTimerCallback(const ros::TimerEvent& event);
};

#endif /* HSM_HEARTBEAT_MONITOR_CLASS_H */
