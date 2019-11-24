#pragma once
#include "iCommand.h"
#include <queue>

class SerialCommand : public iCommand
{
protected:
	std::queue<iCommand*> commands;
public:
	void pushCommand(iCommand*);
	bool isDone();
	void update(float deltaTime);
};

