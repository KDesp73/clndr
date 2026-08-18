#include "cli.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void context_init(Context* ctx, const char* command)
{
    context_reset(ctx);
    ctx->command = command;
    strncpy(ctx->date.fmt, "%Y/%m/%d", sizeof(ctx->date.fmt) - 1);
}

void context_reset(Context* ctx)
{
    context_free(ctx);
    ctx->command = NULL;
}

void context_free(Context* ctx)
{
    if(!ctx) return;
}

void dispatcher_init(Dispatcher* d)
{
    d->count = 0;
}

void dispatcher_add(Dispatcher* d, const char* name, HandlerFunc handler)
{
    d->table[d->count].name = name;
    d->table[d->count].handler = handler;
    d->count++;
}

HandlerFunc dispatcher_find(Dispatcher* d, const char* name)
{
    if(!name) return NULL;
    for(size_t i = 0; i < d->count; ++i) {
        if(!strcmp(d->table[i].name, name))
            return d->table[i].handler;
    }
    return NULL;
}
