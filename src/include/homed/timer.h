#ifndef TIMER_H
#define TIMER_H

struct timer_ctx
{
    struct homed_config *cfg;
    struct member_owner *members;
};

void timer(int (*fn)(void *), void *arg,int seconds);

#endif
