#include "cli.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Command parse_command(const char* str)
{
    if(!str) return COMMAND_NONE;

    if(!strcmp("month", str)) return COMMAND_MONTH;
    else if(!strcmp("until", str)) return COMMAND_UNTIL;

    return COMMAND_NONE;
}

void context_init(Context* ctx, Command command)
{
    context_reset(ctx);
    ctx->command = command;
    strncpy(ctx->date.fmt, "%Y/%m/%d", sizeof(ctx->date.fmt) - 1);
}

void context_reset(Context* ctx)
{
    context_free(ctx);
    ctx->command = COMMAND_NONE;
}

void context_free(Context* ctx)
{
    if(!ctx) return;
}

void set_handler(Dispatcher* this, Command command, HandlerFunc handler)
{
    this->table[command] = handler;
}

HandlerFunc get_handler(Dispatcher* this, Command command)
{
    if(command == COMMAND_NONE) return NULL;
    return this->table[command];
}
