#ifndef CONFIRM_COLLECT_H
#define CONFIRM_COLLECT_H
#include "procedure.h"

class ConfirmCollect : public Procedure
{
public:
	// Members
	uint8_t sampleTypeMux;
	const float backUpDistance = -1.0; // m
	const int confirmCollectValueThreshold = 0.7;
	const int confirmCollectFailedLimit = 3;
	bool noSampleOnGround;
	// Methods
	bool runProc();
};

#endif // CONFIRM_COLLECT_H
