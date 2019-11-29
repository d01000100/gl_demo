#include "ParallelCommand.h"

void ParallelCommand::addCommand(iCommand* c) {
	commands.push_back(c);
}

bool ParallelCommand::isDone() {
	for (int c = 0; c < commands.size(); c++)
		if (!commands[c]->isDone())
			return false;
	
	return true;
}

void ParallelCommand::update(float deltaTime) 
{
	if (!isDone())
		for (int c = 0; c < commands.size(); c++)
			commands[c]->update(deltaTime);
}