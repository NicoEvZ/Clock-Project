/*
	Display.cpp - custom library for handling communication with MAX7221
	7 Segment display driver
	Created: 29/10/2019
	Author: Nico van Zyl
*
	MAX7221 SPI settings:
	- 10MHz communication speed
	- SPI Mode 0 (clock phase and polarity)
	- nCS = pin 25 (PC2)
	- MOSI = pin 15 (PB3)
	- MISO = pin 16 (PB4)
	- SCK = pin 17 (PB5)

	paragraph from https://www.arduino.cc/en/reference/SPI

	"With most SPI devices, after SPI.beginTransaction(), 
	you will write the slave select pin LOW, 
	call SPI.transfer() any number of times to transfer data, 
	then write the SS pin HIGH, and finally call SPI.endTransaction()."
*
	Register Addresses
	0xX0 = No-Op
	0xX1 = Digit 0
		...
	0xX8 = Digit 7
	0xX9 = Decode Mode
	0xXA = Intensity
	0xXB = Scan Limit
	0xXC = Shutdown
	0xXF = Display Test
*
	MAX7221 requires 16 bit MSB packets, with D15-D8 being address,
	and D7-D0 being data.
	
	This will mean the use of SPI.transfer16(), and uint16_t (two byte) variables
*
	MAX7221 intensity register scales from 15/16 to 1/16
*
	MAX7221 can still be programmed when in shutdown mode, which enables the use of
	the shutdown mode as a blinking function.
*/
#include "Display.h"
#include <SPI.h>


Display::Display()
{
	SPI.begin();
}

void Display::setup()
{
	SPI.transfer16(0x09FF); //sets decode mode to Code-B decode for all digits
	SPI.transfer16(0x0A0B); //sets intensity register to 3/4 brightness
	SPI.transfer16(0x0B07); //sets scan limit register to display on all digits
	SPI.transfer16(0x0C01); //sets shutdown register to Normal Operation
	displayOn = true;
}

void Display::toggleDisplay()
{
	if(!displayOn)
	{
		SPI.transfer16(0x0C01); //Normal Operation (turn display on)
		displayOn = true;
	}
	else
	{
		SPI.transfer16(0x0C00); //Shutdown (turn display off)
		displayOn = false;
	}
}

void Display::printPageOne(char timeToDisplay[5])
{
	i = 0;
	for(x=0;x<5;x++)
	{
		splitTime[i]=(((timeToDisplay[x])&0xF0)>>4); //tens unit BCD
		splitTime[i+1]=((timeToDisplay[x])&0x0F); //ones unit BCD
		//seperates BCD time into two chars representing the face value i.e.
		// "26" in BCD to "2" and "6"
		
		if(i==6)//skip positions 8 and 9
		{
			splitTime[8] = 0b00000010;// "2"
			splitTime[9] = 0b00000000;// "0", these two force the "20xx" for the year
			i=10;
		}
		else
			i = i + 2;
	}
	
	for(i=0;i<8;i++)
	{
		SPI.transfer16((positionToDisplay[i]<<8)|splitTime[i]);
	}
}

void Display::printPageTwo(char timeToDisplay[5])
{
	i = 0;
	for(x=0;x<5;x++)
	{
		splitTime[i]=(((timeToDisplay[x])&0xF0)>>4); //tens unit BCD
		splitTime[i+1]=((timeToDisplay[x])&0x0F); //ones unit BCD
		
		if(i==6)//skip positions 8 and 9
		{
			splitTime[8] = 0b00000010;// "2"
			splitTime[9] = 0b00000000;// "0", these two force the "20xx" for the year
			i=10;
		}
		else
			i = i + 2;
	}
	
	for(i=0;1<8;i++)
	{
		SPI.transfer16((positionToDisplay[i]<<8)|timeToDisplay[i+4]);
	}
}

//sets a single digit on the display
void Display::setDigit(char position, char value)
{
	// position will be number from 1 to 8 to determin which 
}