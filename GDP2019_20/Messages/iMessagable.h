#pragma once

#include "Message.h"

class iMessagable {
public:
	virtual void recieveMessage(sMessage message) = 0;
};