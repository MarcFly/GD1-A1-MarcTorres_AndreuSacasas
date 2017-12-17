#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;

	void StartAt(uint32& start_at) { started_at -= start_at; }
	uint32 GetStartedAt() { return started_at; }
private:
	uint32	started_at;
};

#endif //__j1TIMER_H__