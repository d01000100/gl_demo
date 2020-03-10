#include "cLowPassFilter.h"

cLowPassFilter::cLowPassFilter()
{
	// Init the vector
	for ( int count = 0; count != cLowPassFilter::DEFAULTVECTORSIZE; count++ )
	{
		this->vecTimes.push_back( 1.0 / 30.0 );
	}
	this->m_NextLocation = 0;

	return;
}

void cLowPassFilter::addValue(double newValue)
{
	deltaTimeMutex.lock();
	this->vecTimes[this->m_NextLocation] = newValue;
	this->m_NextLocation++;

	// Have I gone off the end of the vector
	if (this->m_NextLocation >= this->vecTimes.size())
	{
		this->m_NextLocation = 0;
	}
	deltaTimeMutex.unlock();
	return;
}
double cLowPassFilter::getAverage(void)
{
	deltaTimeMutex.lock();
	double total = 0.0;
	for (int index = 0; index != this->DEFAULTVECTORSIZE; index++)
	{
		total += this->vecTimes[index];
	}
	total = total / (double)this->DEFAULTVECTORSIZE;
	deltaTimeMutex.unlock();
	return total;
}

