#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include "date.h"
#include <stdbool.h>
#include <stddef.h>

typedef enum {
   ARG_HELP = 'h',
   ARG_VERSION = 'v',
   ARG_DAY = 'D',
   ARG_MONTH = 'M',
   ARG_YEAR = 'Y',
   ARG_DATE = 'd',
   ARG_FMT = 'f',
   ARG_TODAY = 'T',
   ARG_HUMAN = 'H',
} CliArgs;

typedef struct {
    const char* command;
    Date date;
    bool human;
} Context;
void context_init(Context* ctx, const char* command);
void context_reset(Context* ctx);
void context_free(Context* ctx);

typedef bool (*HandlerFunc)(Context);

#define MAX_COMMANDS 32
typedef struct {
    const char* name;
    HandlerFunc handler;
} CommandEntry;

typedef struct {
    CommandEntry table[MAX_COMMANDS];
    size_t count;
} Dispatcher;
void dispatcher_init(Dispatcher* d);
void dispatcher_add(Dispatcher* d, const char* name, HandlerFunc handler);
HandlerFunc dispatcher_find(Dispatcher* d, const char* name);

#endif // CLI_CONFIG_H
