#pragma once

#include <string>

class iCommand
{
private:
	static int nextId;
protected:
	int id;
public:
	std::string name, tag;
	iCommand();
	int getId();
	virtual bool isDone() = 0;
	virtual void update(float deltaTime) = 0;
};