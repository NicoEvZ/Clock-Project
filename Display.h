/*
	Display.h - custom library for handling communication with MAX72221
	7 Segmant display driver
	Created: 29/10/2019
	Author: Nico van Zyl
*/
#ifndef Display_h
#define Display_h

class Display
{
	public:
		Display();
		void setup();
		void setDigit(char position, char value);
		void toggleDisplay();
		void printPageOne(char timeToDisplay[5]);
		void printPageTwo(char timeToDisplay[5]);
	private:
		bool displayOn;
		char i;
		char x;
		char positionToDisplay[8] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
		//digit register addresses
		char splitTime[12];
};

#endif