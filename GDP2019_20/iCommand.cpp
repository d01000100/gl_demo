#include "iCommand.h"

int iCommand::nextId = 0;

iCommand::iCommand() {
	id = nextId++;
}

int iCommand::getId() { return id; }