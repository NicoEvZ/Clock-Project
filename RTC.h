/* 
	RTC.h - Header file of custom RTC library for DS1337.
	created: 24/10/2019
	Author: Nico van Zyl
*/
#ifndef RTC_h
#define RTC_h

class RTC
{
	public:
		RTC();
		void setTime(char timeToSet[5]);
		void setTimeSingle(const char& unit, const char& value);//only sets 
		//a single unit of time i.e. hours
		void setAlarm(char timeToSet[5]);
		void getTime(char timeToDisplay[5]);
		void setup(); //establishes a known time and setting (00:00,00 (1*) 1/1/2000)
		//*DoW is user interpreted, i.e. Mon = 1, Sun = 7
	private:
		const char unitDecode[7] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06};
		const char alarmDecode[3] = {0x0C,0x0B,0x0D};
		const char readOrder[5] = {0x02,0x01,0x04,0x05,0x06};
		//const char setOrder[7] = {0x02,0x01,0x00,0x03,0x04,0x05,0x06};
		char toBCD (char value);
		//char fromBCD (char BCD);
		char x;
		char upperNibble;
		char lowerNibble;
		//char tensUnit;
		//char onesUnit;
};

#endif