
#include "GdkPCH.h"
#include "DateTime.h"

using namespace Gdk;

// ===================================================================================
DateTime::DateTime(struct tm value)
{
	this->internalTime = value;
}

// ===================================================================================
DateTime::DateTime(int year, int month, int day)
{
	memset(this->internalTime, 0, sizeof(struct tm));
	this->internalTime. =
}

// ===================================================================================
DateTime::DateTime(int year, int month, int day, int hour, int minute, int seconds)
{
	this->internalTime = value;
}

// ===================================================================================
DateTime::DateTime(int year, int month, int day, int hour, int minute, int seconds, int milliseconds)
{
	this->internalTime = value;
}