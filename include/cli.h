#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include "date.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
    COMMAND_MONTH,
    COMMAND_UNTIL,
    COMMAND_TODAY,
    COMMAND_LEAP,
    COMMAND_NONE,
} Command;
#define COMMAND_COUNT COMMAND_NONE
Command parse_command(const char* str);

typedef enum {
   ARG_HELP = 'h',
   ARG_VERSION = 'v',
   ARG_DAY = 'D',
   ARG_MONTH = 'M',
   ARG_YEAR = 'Y',
   ARG_DATE = 'd',
   ARG_FMT = 'f',
   ARG_TODAY = 'T',
} CliArgs;

typedef struct {
    Command command;
    Date date;
} Context;
void context_init(Context* ctx, Command command);
void context_reset(Context* ctx);
void context_free(Context* ctx);

typedef bool (*HandlerFunc)(Context);
typedef struct {
   HandlerFunc table[COMMAND_COUNT];
} Dispatcher;
void set_handler(Dispatcher* this, Command command, HandlerFunc handler);
HandlerFunc get_handler(Dispatcher* this, Command command);

#endif // CLI_CONFIG_H
