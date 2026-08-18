#include "cli.h"
#include "date.h"
#include "version.h"
#include <stdlib.h>
#define CLI_IMPLEMENTATION
#include "extern/cli.h"
#define LOGGING_IMPLEMENTATION
#include "extern/logging.h"
#include "cli.h"
#include <stdio.h>

bool cmds_handler(Context ctx)
{
#define COMMAND(cmd, _) printf("%s\n", #cmd);
#include "commands.inc"
#undef COMMANDS
    return true;
}

bool leap_handler(Context ctx)
{
    if(!is_valid_year(ctx.date.year)) {
        ERRO("Invalid year: %zu", ctx.date.year);
        return false;
    }

    bool is_leap = is_leap_year(ctx.date.year);

    if(ctx.human) {
        printf("%zu is %sa leap year\n", ctx.date.year, is_leap ? "" : "not ");
    } else {
        printf("%s\n", is_leap ? "true" : "false");
    }

    return true;
}

bool today_handler(Context ctx)
{
    (void) ctx;

    Date date = {0};
    date_today(&date);
    Weekday weekday = date_weekday(date);

    if(ctx.human) {
        printf("Today is "DATEFMT" and it's a %s\n", DATEARGS(date), weekday_name(weekday));
    } else {
        printf(DATEFMT" %s\n", DATEARGS(date), weekday_name(weekday));
    }
    return true;
}

bool month_handler(Context ctx)
{
    if(!is_valid_day(ctx.date.day)) {
        ERRO("Invalid day: %zu", ctx.date.day);
        return false;
    }
    if(!is_valid_month(ctx.date.month)) {
        ERRO("Invalid month: %zu", ctx.date.month);
        return false;
    }
    if(!is_valid_year(ctx.date.year)) {
        ERRO("Invalid year: %zu", ctx.date.year);
        return false;
    }

    month_print(stdout, ctx.date, true);
    return true;
}

bool until_handler(Context ctx)
{
    if(!is_valid_day(ctx.date.day)) {
        ERRO("Invalid day: %zu", ctx.date.day);
        return false;
    }
    if(!is_valid_month(ctx.date.month)) {
        ERRO("Invalid month: %zu", ctx.date.month);
        return false;
    }
    if(!is_valid_year(ctx.date.year)) {
        ERRO("Invalid year: %zu", ctx.date.year);
        return false;
    }

    Date today = {0};
    date_today(&today);

    int diff = days_between(today, ctx.date);

    if(ctx.human) {
        if(diff > 0) {
            printf("%d days until ", diff);
            date_print(ctx.date);
        } else if(diff < 0) {
            printf("%d days since ", -diff);
            date_print(ctx.date);
        } else {
            printf("Today is ");
            date_print(ctx.date);
        }
    } else {
        if(diff > 0) {
            printf("%d "DATEFMT"\n", diff, DATEARGS(ctx.date));
        } else if(diff < 0) {
            printf("%d "DATEFMT"\n", diff, DATEARGS(ctx.date));
        } else {
            printf("0 "DATEFMT"\n", DATEARGS(ctx.date));
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    cli_args_t args = cli_args_make(
        cli_arg_new(ARG_HELP, "help", "Prints this message", no_argument),
        cli_arg_new(ARG_VERSION, "version", "Prints the version and exits", no_argument),

        cli_arg_new(ARG_DAY, "day", "Specify the day of the month [1-31]", required_argument),
        cli_arg_new(ARG_MONTH, "month", "Specify the month [1-12]", required_argument),
        cli_arg_new(ARG_YEAR, "year", "Specify the year [1582-9999]", required_argument),

        cli_arg_new(ARG_DATE, "date", "Specify the full date (YYYY/MM/DD)", required_argument),
        cli_arg_new(ARG_FMT, "fmt", "Specify the date format (default: \%Y/\%m/\%d)", required_argument),

        cli_arg_new(ARG_TODAY, "today", "Use the current date", no_argument),
        cli_arg_new(ARG_HUMAN, "human", "Human readable output", no_argument),
        NULL
    );
    char* command_str = argc == 1 ? NULL : argv[1];

    Context ctx = {0};
    context_init(&ctx, command_str);

    char* date_input = NULL;
    int opt;
    LOOP_ARGS(opt, args) {
        switch (opt) {
            case ARG_HELP:
                cli_help(args, "calendar <COMMAND> <OPTS>", "Written by KDesp73");
                goto success;
            case ARG_VERSION:
                printf("%s\n", VERSION_STRING);
                goto success;
            case ARG_DAY:
                ctx.date.day = atoi(optarg);
                break;
            case ARG_MONTH:
                ctx.date.month = atoi(optarg);
                break;
            case ARG_YEAR:
                ctx.date.year = atoi(optarg);
                break;
            case ARG_FMT:
                strncpy(ctx.date.fmt, optarg, sizeof(ctx.date.fmt) - 1);
                break;
            case ARG_DATE:
                date_input = optarg;
                strncpy(ctx.date.date, optarg, sizeof(ctx.date.date) - 1);
                break;
            case ARG_TODAY:
                date_today(&ctx.date);
                break;
            case ARG_HUMAN:
                ctx.human = true;
                break;
            default:
                break;
        }
    }

    if(date_input) {
        date_parse(&ctx.date, date_input, ctx.date.fmt);
    }

    Dispatcher dispatcher = {0};
    dispatcher_init(&dispatcher);
#define COMMAND(cmd, h) dispatcher_add(&dispatcher, #cmd, h);
#include "commands.inc"
#undef COMMAND

    HandlerFunc handler = dispatcher_find(&dispatcher, command_str);
    if(!handler) {
        if(!command_str) ERRO("Please provide a command");
        else ERRO("Unknown command: `%s`", command_str);
        goto error;
    }
    if(!handler(ctx)) goto error;

success:
    cli_args_free(&args);
    context_free(&ctx);
    return 0;

error:
    cli_args_free(&args);
    context_free(&ctx);
    return 1;
}
