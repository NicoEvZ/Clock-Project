//DS1337 register addresses
/*
	0x00 -> seconds (00-59)
	0x01 -> minutes (00-59)
	0x02 -> 12/24 hour selection, hours (00-12 +AM/PM or 00-23)
	0x03 -> day (0-7)
	0x04 -> date (01-31)
	0x05 -> centuary, month (01-12 + centuary)
	0x06 -> year (00-99)
*/

#include <RTC.h>
#include <Display.h>
#include <Arduino.h>

Display display;
RTC rtc;

char timeToDisplay[5];

void setup()
{
	rtc.setup();
	display.setup();
}

void loop()
{
	rtc.getTime(timeToDisplay);
	display.printPageOne(timeToDisplay);
}
