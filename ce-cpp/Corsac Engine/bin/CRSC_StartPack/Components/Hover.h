#pragma once

#include "../../CRSC/CRSC.h"

class Hover : public Component
{
public:
	Hover() {};
	~Hover() {};

	void setEvent(bool value)
	{
		click = value;
	}

	bool getEvent()
	{
		return click;
	}

private:
	bool click = false;
};