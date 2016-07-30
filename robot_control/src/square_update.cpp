#include <robot_control/square_update.h>

bool SquareUpdate::runProc()
{
	//ROS_INFO("squareUpdateState = %i",state);
	switch(state)
	{
	case _init_:
		avoidLockout = true;
		procsBeingExecuted[procType] = true;
		procsToExecute[procType] = false;
		computeDriveSpeeds();
		avoidCount = 0;
		numWaypointsToTravel = 5;
		clearAndResizeWTT();
		waypointsToTravel.at(0).x = cornerX;
		waypointsToTravel.at(0).y = cornerY;
		waypointsToTravel.at(1).x = -cornerX;
		waypointsToTravel.at(1).y = cornerY;
		waypointsToTravel.at(2).x = -cornerX;
		waypointsToTravel.at(2).y = -cornerY;
		waypointsToTravel.at(3).x = cornerX;
		waypointsToTravel.at(3).y = -cornerY;
		waypointsToTravel.at(4).x = homeWaypointX;
		waypointsToTravel.at(4).y = homeWaypointY;
		sendDriveAndWait(lidarUpdateWaitTime, true, 180.0);
		//sendDriveRel(0.0, 0.0, true, 180.0, false);
		state = _exec_;
		break;
	case _exec_:
		avoidLockout = false;
		procsBeingExecuted[procType] = true;
		procsToExecute[procType] = false;
		computeDriveSpeeds();
		if(execLastProcType == procType && execLastSerialNum == serialNum) state = _finish_;
		else state = _exec_;
		break;
	case _interrupt_:
		procsBeingExecuted[procType] = false;
		procsToInterrupt[procType] = false;
		state = _exec_;
		break;
	case _finish_:
		avoidLockout = false;
		if(robotStatus.homingUpdated)
		{
			timers[_homingTimer_]->stop();
			timers[_homingTimer_]->start();
			performHoming = false;
			homingUpdatedFailedCount = 0;
			homingUpdateFailed = false;
			useDeadReckoning = false;
		}
		else
		{
			homingUpdatedFailedCount++;
			homingUpdateFailed = true;
			if(homingUpdatedFailedCount>=homingFailedSwitchToDeadReckoningCount)
			{
				useDeadReckoning = true; // !!! This switch in hsm probably won't happen quickly enough before the next cycle of planning a manuever. How to handle this?
			}
			if(homingUpdatedFailedCount>=maxHomingUpdatedFailedCount)
			{
				performSafeMode = true;
			}
		}
		atHome = true;
		procsBeingExecuted[procType] = false;
		procsToExecute[procType] = false;
		state = _init_;
		break;
	}
}
