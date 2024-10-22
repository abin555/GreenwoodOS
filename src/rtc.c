#include "rtc.h"
#include "io.h"
#include "timer.h"
#include "serial.h"

#define CURRENT_YEAR        2023                            // Change this each year!

int century_register = 0x00;                                // Set by ACPI table parsing code if possible

struct RealTimeClock RTC;

enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};

int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}

unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}

void read_rtc() {
      unsigned char century;
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;

      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates

      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      RTC.second = get_RTC_register(0x00);
      RTC.minute = get_RTC_register(0x02);
      RTC.hour = get_RTC_register(0x04);
      RTC.day = get_RTC_register(0x07);
      RTC.month = get_RTC_register(0x08);
      RTC.year = get_RTC_register(0x09);
      if(century_register != 0) {
            century = get_RTC_register(century_register);
      }

      do {
            last_second = RTC.second;
            last_minute = RTC.minute;
            last_hour = RTC.hour;
            last_day = RTC.day;
            last_month = RTC.month;
            last_year = RTC.year;
            last_century = century;

            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            RTC.second = get_RTC_register(0x00);
            RTC.minute = get_RTC_register(0x02);
            RTC.hour = get_RTC_register(0x04);
            RTC.day = get_RTC_register(0x07);
            RTC.month = get_RTC_register(0x08);
            RTC.year = get_RTC_register(0x09);
            if(century_register != 0) {
                  century = get_RTC_register(century_register);
            }
      } while( (last_second != RTC.second) || (last_minute != RTC.minute) || (last_hour != RTC.hour) ||
               (last_day != RTC.day) || (last_month != RTC.month) || (last_year != RTC.year) ||
               (last_century != century) );

      registerB = get_RTC_register(0x0B);

      // Convert BCD to binary values if necessary

      if (!(registerB & 0x04)) {
            RTC.second = (RTC.second & 0x0F) + ((RTC.second / 16) * 10);
            RTC.minute = (RTC.minute & 0x0F) + ((RTC.minute / 16) * 10);
            RTC.hour = ((RTC.hour & 0x0F) + (((RTC.hour & 0x70) / 16) * 10) ) | (RTC.hour & 0x80);
            RTC.day = (RTC.day & 0x0F) + ((RTC.day / 16) * 10);
            RTC.month = (RTC.month & 0x0F) + ((RTC.month / 16) * 10);
            RTC.year = (RTC.year & 0x0F) + ((RTC.year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }

      // Convert 12 hour clock to 24 hour clock if necessary

      if (!(registerB & 0x02) && (RTC.hour & 0x80)) {
            RTC.hour = ((RTC.hour & 0x7F) + 12) % 24;
      }

      // Calculate the full (4-digit) year

      if(century_register != 0) {
            RTC.year += century * 100;
      } else {
            RTC.year += (CURRENT_YEAR / 100) * 100;
            if(RTC.year < CURRENT_YEAR) RTC.year += 100;
      }
      RTC.msec += 100;
}

void rtc_callback(){
	read_rtc();
	//print_serial("[RTC] %d:%d:%d\n", RTC.hour, RTC.minute, RTC.second);
}

void rtc_init(){
	timer_attach(50, rtc_callback);
}