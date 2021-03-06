#include <robot_control/drive_straight_cl.h>

void DriveStraightCL::init()
{
	initX_ = robotStatus.xPos;
	initY_ = robotStatus.yPos;
	initHeading_ = robotStatus.heading;
	desiredDistance_ = params.float1;
	if(desiredDistance_<0.0) driveSign_ = -1;
	else driveSign_ = 1;
	timeoutValue_ = (unsigned int)round((30.0 + 1.0*fabs(desiredDistance_))*robotStatus.loopRate);
	timeoutCounter_ = 0;
	headingErrorSpeedI_ = 0.0;
	inThreshold_ = false;
	thresholdTime_ = 0.0;
    robotOutputs.stopFlag = false;
    robotOutputs.turnFlag = false;
}

int DriveStraightCL::run()
{
    robotOutputs.stopFlag = false;
    robotOutputs.turnFlag = false;
    vMax_ = robotStatus.vMax;
	traversedDistance_ = driveSign_*hypot(robotStatus.xPos-initX_,robotStatus.yPos-initY_);
	remainingDistance_ = desiredDistance_ - traversedDistance_;
	vDesRaw_ = kpV_*remainingDistance_;
	if(vDesRaw_>0.0) vDesCoerc_ = vDesRaw_+vMin_;
	else if(vDesRaw_<0.0) vDesCoerc_ = vDesRaw_-vMin_;
	else vDesCoerc_ = vDesRaw_;
	if(vDesCoerc_>vMax_) vDesCoerc_ = vMax_;
	else if(vDesCoerc_<(-vMax_)) vDesCoerc_ = -vMax_;
	deltaHeading_ = initHeading_ - robotStatus.heading;
	rDes_ = kpR_*deltaHeading_;
	if(rDes_>rMax_) rDes_ = rMax_;
	else if(rDes_<(-rMax_)) rDes_ = -rMax_;
    if(std::isnan(robotStatus.yawRate)) {ROS_ERROR("yaw rate is nan"); robotStatus.yawRate = yawRatePrev_;}
    else yawRatePrev_ = robotStatus.yawRate;
	errorR_ = rDes_ - robotStatus.yawRate;
	headingErrorSpeedP_ = kpR_*rDes_;
    headingErrorSpeedI_ += kiR_*errorR_;
	if(headingErrorSpeedI_>maxHeadingErrorSpeedI_) headingErrorSpeedI_ = maxHeadingErrorSpeedI_;
	else if(headingErrorSpeedI_<(-maxHeadingErrorSpeedI_)) headingErrorSpeedI_ = -maxHeadingErrorSpeedI_;
	headingErrorSpeedT_ = headingErrorSpeedP_ + headingErrorSpeedI_;
	if(headingErrorSpeedT_>maxHeadingErrorSpeed_) headingErrorSpeedT_ = maxHeadingErrorSpeed_;
	else if(headingErrorSpeedT_<(-maxHeadingErrorSpeed_)) headingErrorSpeedT_ = -maxHeadingErrorSpeed_;
	leftSpeed_ = round(kVOutput_*vDesCoerc_+headingErrorSpeedT_);
	rightSpeed_ = round(kVOutput_*vDesCoerc_-headingErrorSpeedT_);
	timeoutCounter_++;
	if(fabs(remainingDistance_)<=distanceThreshold_ && inThreshold_==false) {thresholdInitTime_ = ros::Time::now().toSec(); inThreshold_ = true;}
	if(fabs(remainingDistance_)<=distanceThreshold_) thresholdTime_ = ros::Time::now().toSec() - thresholdInitTime_;
	else {thresholdTime_ = 0.0; inThreshold_ = false;}
	if(thresholdTime_ >= thresholdMinTime_ || timeoutCounter_ >= timeoutValue_)
	{
		robotOutputs.flMotorSpeed = 0;
		robotOutputs.mlMotorSpeed = 0;
		robotOutputs.blMotorSpeed = 0;
		robotOutputs.frMotorSpeed = 0;
		robotOutputs.mrMotorSpeed = 0;
		robotOutputs.brMotorSpeed = 0;
		taskEnded_ = 1;
	}
	else
	{
		robotOutputs.flMotorSpeed = leftSpeed_;
		robotOutputs.mlMotorSpeed = leftSpeed_;
		robotOutputs.blMotorSpeed = leftSpeed_;
		robotOutputs.frMotorSpeed = rightSpeed_;
		robotOutputs.mrMotorSpeed = rightSpeed_;
		robotOutputs.brMotorSpeed = rightSpeed_;
		taskEnded_ = 0;
	}
	return taskEnded_;
}
