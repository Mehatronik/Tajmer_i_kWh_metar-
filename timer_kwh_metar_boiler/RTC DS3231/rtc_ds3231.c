/*
 * rtc_ds3231.c
 *
 * Created: 28.4.2019. 15.07.39
 *  Author: Kovacevic
 */ 

#include "comm.h"
#include "rtc_ds3231.h"
#include "i2cmaster.h"

unsigned char bcd_to_decimal(unsigned char d)
{
	return ((d & 0x0F) + (((d & 0xF0) >> 4) * 10));
}


unsigned char decimal_to_bcd(unsigned char d)
{
	return (((d / 10) << 4) & 0xF0) | ((d % 10) & 0x0F);
}


unsigned char DS3231_Read(unsigned char address)
{
	unsigned char value = 0;
	
	i2c_start_wait(DS3231_Write_addr + I2C_WRITE);	// set device address and write mode
	i2c_write(address);
	i2c_start_wait(DS3231_Write_addr + I2C_READ);    // set device address and read mode
	value = i2c_readNak();							 //read one byte, read is followed by a stop condition

	return value;
}


void DS3231_Write(unsigned char address, unsigned char value)
{

	i2c_start_wait(DS3231_Write_addr + I2C_WRITE);	// set device address and write mode
	i2c_write(address);
	i2c_write(value);
	i2c_stop();
	
}


void DS3231_init()
{
	
	DS3231_Write(controlREG, 0x00);   //oscilator ON on BAT;  EOSC, BBSQW, CONV, RS2, RS1, INTCN, A2IE, A1IE = 0 (datasheet ds3231 za detalje)
	DS3231_Write(statusREG, 0x00);	  //32khz out off;  1<<EN32kHz - ON
	DS3231_Write(ageoffsetREG, (unsigned char)(-24));	//negativne vrednosti ubrzavaju. Izmerio sam da svaki bit ima uticaj 0.042ppm. Izmereno odstupanje RTCa je -1ppm
														//logika je 2s complement; jednostavno castujem, ili mogu reci:
														//if offset <0 THEN offset = offset + 256. Ako su pozitivne vrednosti jednostavno upisem
} 


void getTime(unsigned char *p3, unsigned char *p2, unsigned char *p1, unsigned char *p0, unsigned char hour_format)
{
	
	//getTime(&vreme_datum.hr, &vreme_datum.min, &vreme_datum.s, &vreme_datum.am_pm, _24_hour_format);
	//dakle, *p3=sat, *p2=minut, *p1=sekund, *p0=am/pm
	
	
	unsigned char tmp = 0;
	
	tmp = DS3231_Read(secondREG);
	*p1 = bcd_to_decimal(tmp);
	tmp = DS3231_Read(minuteREG);
	*p2 = bcd_to_decimal(tmp);
	
	switch(hour_format)
	{
		case 1:
		{
			tmp = DS3231_Read(hourREG);
			tmp &= 0x20;
			*p0 = (unsigned char)(tmp >> 5);
			tmp = (0x1F & DS3231_Read(hourREG));
			*p3 = bcd_to_decimal(tmp);
			break;
		}
		default:
		{
			tmp = (0x3F & DS3231_Read(hourREG));
			*p3 = bcd_to_decimal(tmp);
			break;
		}
	}
}


void getDate(unsigned char *p4, unsigned char *p3, unsigned char *p2, unsigned char *p1)
{
	unsigned char tmp = 0;
	
	tmp = DS3231_Read(yearREG);
	*p1 = bcd_to_decimal(tmp);
	tmp = (0x1F & DS3231_Read(monthREG));
	*p2 = bcd_to_decimal(tmp);
	tmp = (0x3F & DS3231_Read(dateREG));
	*p3 = bcd_to_decimal(tmp);
	tmp = (0x07 & DS3231_Read(dayREG));
	*p4 = bcd_to_decimal(tmp);
}


void setTime(unsigned char hSet, unsigned char mSet, unsigned char sSet, unsigned char am_pm_state, unsigned char hour_format)
{
	unsigned char tmp = 0;
	
	DS3231_Write(secondREG, (decimal_to_bcd(sSet)));
	DS3231_Write(minuteREG, (decimal_to_bcd(mSet)));
	
	switch(hour_format)
	{
		case 1:
		{
			switch(am_pm_state)
			{
				case 1:
				{
					tmp = 0x60;
					break;
				}
				default:
				{
					tmp = 0x40;
					break;
				}
			}
			DS3231_Write(hourREG, ((tmp | (0x1F & (decimal_to_bcd(hSet))))));
			break;
		}
		
		default:
		{
			DS3231_Write(hourREG, (0x3F & (decimal_to_bcd(hSet))));
			break;
		}
	}
}


void setDate(unsigned char daySet, unsigned char dateSet, unsigned char monthSet, unsigned char yearSet)
{
	DS3231_Write(dayREG, (decimal_to_bcd(daySet)));
	DS3231_Write(dateREG, (decimal_to_bcd(dateSet)));
	DS3231_Write(monthREG, (decimal_to_bcd(monthSet)));
	DS3231_Write(yearREG, (decimal_to_bcd(yearSet)));
}


void setA1Time(unsigned char hSet, unsigned char mSet, unsigned char am_pm_state, unsigned char hour_format)
{
	unsigned char tmp = 0;
	
	DS3231_Write(alarm1secREG , (0x7F & (decimal_to_bcd(0))));
	DS3231_Write(alarm1minREG , (0x7F & (decimal_to_bcd(mSet))));
	
	switch(hour_format)
	{
		case 1:
		{
			switch(am_pm_state)
			{
				case 1:
				{
					tmp = 0x60;
					break;
				}
				default:
				{
					tmp = 0x40;
					break;
				}
			}
			DS3231_Write(alarm1hrREG, ((tmp | (0x1F & (decimal_to_bcd(hSet))))));
			break;
		}
		
		default:
		{
			DS3231_Write(alarm1hrREG, (0x3F & (decimal_to_bcd(hSet))));
			break;
		}
	}
}


void setA2Time(unsigned char hSet, unsigned char mSet, unsigned char am_pm_state, unsigned char hour_format)
{
	unsigned char tmp = 0;
	
	DS3231_Write(alarm2minREG , (0x7F & (decimal_to_bcd(mSet))));
	
	switch(hour_format)
	{
		case 1:
		{
			switch(am_pm_state)
			{
				case 1:
				{
					tmp = 0x60;
					break;
				}
				default:
				{
					tmp = 0x40;
					break;
				}
			}
			DS3231_Write(alarm2hrREG, ((tmp | (0x1F & (decimal_to_bcd(hSet))))));
			break;
		}
		
		default:
		{
			DS3231_Write(alarm2hrREG, (0x3F & (decimal_to_bcd(hSet))));
			break;
		}
	}
}


void getA1Time(unsigned char *p2, unsigned char *p1, unsigned char *p0, unsigned char hour_format)
{
	unsigned char tmp = 0;
	
	tmp = DS3231_Read(alarm1minREG);
	*p1 = bcd_to_decimal(tmp);
	
	switch(hour_format)
	{
		case 1:
		{
			tmp = DS3231_Read(alarm1hrREG);
			tmp &= 0x20;
			*p0 = (unsigned char)(tmp >> 5);
			tmp = (0x1F & DS3231_Read(alarm1hrREG));
			*p2 = bcd_to_decimal(tmp);
			break;
		}
		default:
		{
			tmp = (0x3F & DS3231_Read(alarm1hrREG));
			*p2 = bcd_to_decimal(tmp);
			break;
		}
	}
}


void getA2Time(unsigned char *p2, unsigned char *p1, unsigned char *p0, unsigned char hour_format)
{
	unsigned char tmp = 0;
	
	tmp = DS3231_Read(alarm2minREG);
	*p1 = bcd_to_decimal(tmp);
	
	switch(hour_format)
	{
		case 1:
		{
			tmp = DS3231_Read(alarm2hrREG);
			tmp &= 0x20;
			*p0 = (unsigned char)(tmp >> 5);
			tmp = (0x1F & DS3231_Read(alarm2hrREG));
			*p2 = bcd_to_decimal(tmp);
			break;
		}
		default:
		{
			tmp = (0x3F & DS3231_Read(alarm2hrREG));
			*p2 = bcd_to_decimal(tmp);
			break;
		}
	}
}


float getTemp()
{
	float t = 0.0;
	unsigned char lowByte = 0;
	signed char highByte = 0;
	lowByte = DS3231_Read(tempLSBREG);
	highByte = DS3231_Read(tempMSBREG);
	lowByte >>= 6;
	lowByte &= 0x03;
	t = ((float)lowByte);
	t *= 0.25;
	t += highByte;
	return t;
	
}