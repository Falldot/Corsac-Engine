#pragma once

#include "../../CRSC/CRSC.h"

class ClickDown : public Component
{
public:
	ClickDown() {};
	~ClickDown() {};

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