#ifndef DATE_H
#define DATE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t day;
    size_t month;
    size_t year;
    char date[10];
    char fmt[10];
} Date;

#define DATEFMT "%zu/%zu/%zu"
#define DATEARGS(date) (date).year, (date).month, (date).day

typedef enum {
    JANUARY = 1,
    FEBRUARY,
    MARCH,
    APRIL,
    MAY,
    JUNE,
    JULY,
    AUGUST,
    SEPTEMBER,
    OCTOBER,
    NOVEMBER,
    DECEMBER
} Month;

static char month_names[][16] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

#define month_name(month) month_names[(month)-1]
#define month_char(month) month_names[(month)-1][0]

typedef enum {
    MONDAY,
    TUESDAY,
    WEDNESDAY,
    THURSDAY,
    FRIDAY,
    SATURDAY,
    SUNDAY
} Weekday;

static char weekday_names[][16] = {
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday",
    "Sunday"
};

#define weekday_name(weekday) weekday_names[(weekday)]
#define weekday_char(weekday) weekday_names[(weekday)][0]
#define WEEKDAY_CHARS "MTWTFSS"

#define is_valid_day(day) ((day) >= 1 && (day) <= 31)
#define is_valid_month(month) ((month) >= 1 && (month) <= 12)
#define is_valid_year(year) ((year) >= 1582 && (year) <= 9999)

bool is_leap_year(size_t year);
size_t month_days(size_t year, Month month);

void date_print(Date date);
void date_parse(Date* date, char* input, char* fmt);
Weekday date_weekday(Date date);
void month_print(FILE* stream, Date date, bool highlight_day);
void date_today(Date* date);
int days_between(Date from, Date to);
void date_add_months(Date* date, int months);

#endif // DATE_H
