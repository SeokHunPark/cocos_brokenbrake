#include "Vehicle.h"

Vehicle::Vehicle()
{

}

Vehicle::~Vehicle()
{
	release();
}

void Vehicle::release()
{
	if (NULL != body)
	{
		body->removeFromParentAndCleanup(true);
		body = NULL;
	}
}