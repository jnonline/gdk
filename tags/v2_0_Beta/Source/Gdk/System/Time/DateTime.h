#pragma once

namespace Gdk
{
	// ============================================================================

	namespace DayOfWeek
	{
		enum Enum
		{
			Sunday,
			Monday,
			Tuesday,
			Wednesday,
			Thursday,
			Friday,
			Saturday
		}

		// Gets the string equivalent of the given day of the week
		const char* ToString(Enum dayOfWeek)
		{
			switch(dayOfWeek)
			{
				case Sunday:	static const char* sunday = "Sunday";		return sunday;
				case Monday:	static const char* monday = "Monday";		return monday;
				case Tuesday:	static const char* tuesday = "Tuesday";		return tuesday;
				case Wednesday: static const char* wednesday = "Wednesday"; return wednesday;
				case Thursday:	static const char* thursday = "Thursday";	return thursday;
				case Friday:	static const char* friday = "Friday";		return friday;
				case Saturday:	static const char* saturday = "Saturday";	return saturday;
			}
		}
	}

	// ============================================================================

	class DateTime
	{
	public: 

		// Instance Methods
		// --------------------------------------------

		// CTor
		DateTime(struct tm value);
		DateTime(int year, int month, int day);
		DateTime(int year, int month, int day, int hour, int minute, int seconds);
		DateTime(int year, int month, int day, int hour, int minute, int seconds, int milliseconds);

		// Time manipulation methods
		void AddYears(int years);
		void AddMonths(int months);
		void AddDays(int days);
		void AddHours(int hours);
		void AddMinutes(int minutes);
		void AddSeconds(int seconds);
		void AddMilliseconds(int milliseconds);

		// Static Methods
		// --------------------------------------------

		// Gets the current local time
		DateTime GetCurrent();

		// Gets the current UTC time
		DateTime GetCurrentUTC();
		

	private:

		// Internals
		// --------------------------------------------

		struct tm internalTime;
		
	};

} // namespace Gdk
