#include "cMesh.h"

void cMesh::setLoadState(eLoadState state)
{
	loadStateLock.lock();
	loadState = state;
	loadStateLock.unlock();
}
cMesh::eLoadState cMesh::getLoadState()
{
	loadStateLock.lock();
	eLoadState res = loadState;
	loadStateLock.unlock();
	return res;
}