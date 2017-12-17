#ifndef _UI_GROUP_H_
#define _UI_GROUP_H_

#include "UI_Elements.h"
#include "j1Module.h"

class UI_Group  : public j1Module
{
public:
	bool PreUpdate()
	{
		bool ret = true;

		return ret;
	}

	bool PostUpdate()
	{
		bool ret = true;

		return ret;
	}

	bool CleanUp()
	{
		bool ret = true;

		return ret;
	}


public:
	p2List<UI_Element*> objects;
};


#endif //_UI_GROUP_H_