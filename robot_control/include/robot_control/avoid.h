#ifndef AVOID_H
#define AVOID_H
#include "procedure.h"
#include "action_type_enum.h"

class Avoid : public Procedure
{
	// Members
	bool interrupted = false;
	// Methods
	bool runProc();
};

#endif // AVOID_H
