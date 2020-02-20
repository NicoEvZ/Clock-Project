/* 
	RTC.cpp - Source file of custom RTC library for DS1337.
	created: 24/10/2019
	Author: Nico van Zyl

DS1337 register addresses

	0x00 -> seconds (00-59) ****will not be using seconds***
	0x01 -> minutes (00-59)
	0x02 -> 12/24 hour selection, hours (00-12 +AM/PM or 00-23)
	0x03 -> day (0-7)
	0x04 -> date (01-31)
	0x05 -> centuary, month (01-12 + centuary)
	0x06 -> year (00-99)
*/ 

// arrays used to hold RTC register addresses for easy incrementing
//const char unitDecode[7] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06};
//const char alarmDecode[5] = {0x07,0x08,0x09,0x0A,0x0B};

//const char readOrder[5] = {0x02,0x01,0x04,0x05,0x06};

// constructor will run every time an instance of the class is created

//#include "Arduino.h"

#include "RTC.h"
#include <Wire.h>
#define RTCADDR 0b1101000


RTC::RTC()
{
	Wire.begin();
}

void RTC::setup()
{
	Wire.beginTransmission(RTCADDR);
	Wire.write(0x00); //sets the address pointer to the top
	Wire.write(0x00); //data for seconds
	Wire.write(0x00); //data for minutes
	Wire.write(0x00); //data for hours/sets 24 hour time
	Wire.write(0x01); //data for DoW(1-7)
	Wire.write(0x01); //data for DoM(1-31)
	Wire.write(0x01); //data for MoY(1-12)
	Wire.write(0x00); //data for Year
	Wire.endTransmission();
	Wire.beginTransmission(RTCADDR);
	Wire.write(0x0E); //set address pointer to control register
	Wire.write(0b10000110); //data for control register, enable oscilator, enable alarm 2
	Wire.endTransmission();
	//time and date initialised as 00:00,00 (1*) 1/1/2000
	//*DoW is user interpreted, i.e. Mon = 1, Sun = 7
	
	/*
		DS1337 automatically increments which register is being written to,
		which is why there is only one register adress write.
	*/
}

void RTC::setTimeSingle(const char& unit, const char& value)
{
	//useing Wire to cue bytes and then transmit them to the RTC
	Wire.beginTransmission(RTCADDR);
	Wire.write(unitDecode[unit]);
	Wire.write(toBCD(value));
	Wire.endTransmission();
}

void RTC::setTime(char timeToSet[5])
{
	Wire.beginTransmission(RTCADDR);
	Wire.write(0x00); //seconds register adress
	Wire.write(0x00); //reset seconds to 0 for user set time.
	Wire.endTransmission();
	
	for(x=0;x<5;x++)
	{
		//useing Wire to cue bytes and then transmit them to the RTC
		Wire.beginTransmission(RTCADDR);
		Wire.write(readOrder[x]); //address pointer set to correct unit
		Wire.write(timeToSet[x]); //value written to the data register
		Wire.endTransmission();
	}
}

void RTC::getTime(char timeToDisplay[5])
{
	//DS1337 able to dump multiple bytes
	//write register pointer, then request read
	//read will continue until master gives NACK
	//timeToDisplay will be an array, 5 units, created in main.
	//the array will be packed in RTC.getTime for easy display.
	for(x=0;x<5;x++)
	{
		Wire.beginTransmission(RTCADDR);
		Wire.write(readOrder[x]); //set address pointer for read
		Wire.endTransmission();
		
		Wire.requestFrom(RTCADDR, 1);
		while(Wire.available())
		{
			timeToDisplay[x] = Wire.read();
		}
	}
	//timeToDisplay will contain:{hrs,min,DoM,MoY,Yr(2Dig)}
}

// Alarm 2 will be used, as its lowest resolution is minutes
void RTC::setAlarm(char timeToSet[5])
{
	for(x=0;x<3;x++)
	{
		Wire.beginTransmission(RTCADDR);
		Wire.write(alarmDecode[x]); //address pointer set to correct unit
		Wire.write(timeToSet[x]); //value written to the data register
		Wire.endTransmission();
	}
}

char RTC::toBCD (char value)
{
	upperNibble = (value / 10) << 4;
	lowerNibble = value % 10;
	return upperNibble|lowerNibble;
}

/*
char RTC::fromBCD (char BCD)
{
	tensUnit = ((BCD & 0xF0) >> 4) * 10;
	onesUnit = BCD & 0x0F;
	return tensUnit + onesUnit;
}
*/