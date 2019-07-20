/*
 * rtc_ds3231.h
 *
 * Created: 28.4.2019. 15.07.18
 *  Author: Kovacevic
 */ 


#ifndef RTC_DS3231_H_
#define RTC_DS3231_H_

#define DS3231_Address              0x68

#define DS3231_Read_addr            ((DS3231_Address << 1) | 0x01)
#define DS3231_Write_addr           ((DS3231_Address << 1) & 0xFE)

#define secondREG                   0x00
#define minuteREG                   0x01
#define hourREG                     0x02
#define dayREG                      0x03
#define dateREG                     0x04
#define monthREG                    0x05
#define yearREG                     0x06
#define alarm1secREG                0x07
#define alarm1minREG                0x08
#define alarm1hrREG                 0x09
#define alarm1dateREG               0x0A
#define alarm2minREG                0x0B
#define alarm2hrREG                 0x0C
#define alarm2dateREG               0x0D
#define controlREG                  0x0E
#define statusREG                   0x0F
#define ageoffsetREG                0x10
#define tempMSBREG                  0x11
#define tempLSBREG                  0x12

/* Control register(0x0E) bits */
#define EOSC  7
#define BBSQW 6
#define CONV  5
#define RS2   4
#define RS1   3
#define INTCN 2
#define A2IE  1
#define A1IE  0

/* Status register(0x0F) bits */
#define OSF     7
#define EN32kHz 3
#define BSY     2
#define A2F     1
#define A1F     0


#define _24_hour_format             0
#define _12_hour_format             1
#define am                          0
#define pm                          1


extern unsigned char bcd_to_decimal(unsigned char d);
extern unsigned char decimal_to_bcd(unsigned char d);
extern unsigned char DS3231_Read(unsigned char address);
void DS3231_Write(unsigned char address, unsigned char value);
extern void DS3231_init();
extern void getTime(unsigned char *p3, unsigned char *p2, unsigned char *p1, unsigned char *p0, unsigned char hour_format);
extern void getDate(unsigned char *p4, unsigned char *p3, unsigned char *p2, unsigned char *p1);
extern void setTime(unsigned char hSet, unsigned char mSet, unsigned char sSet, unsigned char am_pm_state, unsigned char hour_format);
extern void setDate(unsigned char daySet, unsigned char dateSet, unsigned char monthSet, unsigned char yearSet);
extern void setA1Time(unsigned char hSet, unsigned char mSet, unsigned char am_pm_state, unsigned char hour_format);
extern void setA2Time(unsigned char hSet, unsigned char mSet, unsigned char am_pm_state, unsigned char hour_format);
extern void getA1Time(unsigned char *p2, unsigned char *p1, unsigned char *p0, unsigned char hour_format);
extern void getA2Time(unsigned char *p2, unsigned char *p1, unsigned char *p0, unsigned char hour_format);
extern float getTemp();



#endif /* RTC_DS3231_H_ */