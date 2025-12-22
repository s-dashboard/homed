#ifndef TIMER_H
#define TIMER_H

struct timer_ctx
{
    struct homed_config *cfg;
    struct member_owner *members;
    struct presence_list *presence;
};

struct thread_args {
    int (*fn)(void *);
    void *arg;
    int seconds;
};

void timer(int (*fn)(void *), void *arg,int seconds);
void *timer_thread(void *arg);

#endif
