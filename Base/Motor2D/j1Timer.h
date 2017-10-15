#ifndef __j1TIMER_H__
#define __j1TIMER_H__

class j1Timer
{
public:

	// Constructors
	j1Timer();
	~j1Timer();

	void Start();
	unsigned int Read() const;
	float ReadSec() const;

private:
	unsigned int started_at;
};

#endif //__j1TIMER_H__