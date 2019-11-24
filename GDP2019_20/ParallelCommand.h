#pragma once
#include "iCommand.h"

#include <vector>

class ParallelCommand : public iCommand
{
protected:
	std::vector<iCommand*> commands;
public:
	void addCommand(iCommand *c);
	bool isDone();
	void update(float deltaTime);
};

