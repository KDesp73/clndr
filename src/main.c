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
        NULL
    );
    char* command_str = argc == 1 ? NULL : argv[1];
    Command command = parse_command(command_str);

    Context ctx = {0};
    context_init(&ctx, command);

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
                date_parse(&ctx.date, optarg, ctx.date.fmt);
                strncpy(ctx.date.date, optarg, sizeof(ctx.date.date) - 1);
                break;
            case ARG_TODAY:
                date_today(&ctx.date);
                break;
            default:
                break;
        }
    }

    Dispatcher dispatcher = {0};
    set_handler(&dispatcher, COMMAND_MONTH, month_handler);

    HandlerFunc handler = get_handler(&dispatcher, command);
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
