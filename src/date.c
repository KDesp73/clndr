#include "date.h"
#include "extern/ansi.h"
#include <stdio.h>
#include <string.h>

void date_print(Date date)
{
    printf(DATEFMT"\n", DATEARGS(date)); 
}

Weekday date_weekday(Date date)
{
    size_t y = date.year;
    size_t m = date.month;
    size_t d = date.day;
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (m < 3) y--;
    int w = (y + y/4 - y/100 + y/400 + t[m-1] + d) % 7;
    // Sakamoto: 0=Sun,1=Mon,...,6=Sat -> shift to 0=Mon,...,6=Sun
    return (w + 6) % 7;
}

bool is_leap_year(size_t year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

size_t month_days(size_t year, Month month)
{
    if(month == FEBRUARY)
        return is_leap_year(year) ? 29 : 28;
    if(month == APRIL || month == JUNE || month == SEPTEMBER || month == NOVEMBER)
        return 30;
    return 31;
}

void month_print(FILE* stream, Date date, bool highlight_day)
{
    size_t total_days = month_days(date.year, date.month);

    Date first_of_month = date;
    first_of_month.day = 1;
    Weekday start_weekday = date_weekday(first_of_month);

    // Print header
    fprintf(stream, "%20s\n", month_name(date.month));
    for (size_t i = 0; i < strlen(WEEKDAY_CHARS); ++i) {
        fprintf(stream, "%2c ", WEEKDAY_CHARS[i]);
    }
    fprintf(stream, "\n");

    // Print leading padding spaces for the first week
    for (int i = 0; i < start_weekday; ++i) {
        fprintf(stream, "   ");
    }

    // Print days
    size_t current_pos = start_weekday;
    for (size_t day = 1; day <= total_days; ++day) {
        if (highlight_day && day == date.day) {
            fprintf(stream, "%s%2zu%s ", ANSI_RED, day, ANSI_RESET);
        } else {
            fprintf(stream, "%2zu ", day);
        }

        current_pos++;
        if (current_pos % 7 == 0) {
            fprintf(stream, "\n");
        }
    }

    if (current_pos % 7 != 0) {
        fprintf(stream, "\n");
    }
}

void date_parse(Date* date, char* input, char* fmt)
{
    if (!date || !input || !fmt) return;

    char scan_fmt[64] = {0};
    size_t scan_idx = 0;
    
    char order[3] = {0};
    size_t order_count = 0;

    for (size_t i = 0; fmt[i] != '\0' && scan_idx < sizeof(scan_fmt) - 5; ++i) {
        if (fmt[i] == '%') {
            char spec = fmt[++i];
            if (spec == 'd' || spec == 'm' || spec == 'Y' || spec == 'y') {
                strcat(scan_fmt + scan_idx, "%zu");
                scan_idx += 3;
                if (order_count < 3) {
                    order[order_count++] = (spec == 'Y' || spec == 'y') ? 'y' : spec;
                }
            } else if (spec == '%') {
                scan_fmt[scan_idx++] = '%';
            }
        } else {
            scan_fmt[scan_idx++] = fmt[i];
        }
    }

    size_t val1 = 0, val2 = 0, val3 = 0;
    int parsed = sscanf(input, scan_fmt, &val1, &val2, &val3);

    if (parsed < (int)order_count) return;

    size_t* vals[3] = { &val1, &val2, &val3 };
    for (size_t i = 0; i < order_count; ++i) {
        if (order[i] == 'd') date->day = *vals[i];
        else if (order[i] == 'm') date->month = *vals[i];
        else if (order[i] == 'y') date->year = *vals[i];
    }
}

#include <time.h>

void date_today(Date* date)
{
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    date->day = tm.tm_mday;
    date->month = tm.tm_mon + 1;
    date->year = tm.tm_year + 1900;
}
