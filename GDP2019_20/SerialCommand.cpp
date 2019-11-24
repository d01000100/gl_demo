#include "SerialCommand.h"

void SerialCommand::pushCommand(iCommand* c) {
	commands.push(c);
}

bool SerialCommand::isDone() {
	return commands.empty();
}

void SerialCommand::update(float deltaTime)
{
	if (!isDone())
	{
		iCommand* current = commands.front();
		if (!current->isDone())
		{
			current->update(deltaTime);
		}
		else
		{
			commands.pop();
		}
	}
}