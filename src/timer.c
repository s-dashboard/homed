#include <homed/timer.h>
#include <homed/wifi_clients.h>
#include <homed/members.h>
#include <unistd.h>
#include <stdio.h>

void timer(int (*fn)(void *), void *arg, int seconds) {
    while(1) {
        int error = fn(arg);
        if(error != 0) break;
        
        sleep(seconds); 
    }
}

// TODO: Move over to own threads and remove the old timer above. 
void *timer_thread(void *arg)
{
    struct thread_args *t = arg;

    while (1) {
        int err = t->fn(t->arg);
        if (err != 0)
            break;

        sleep(t->seconds);
    }

    return NULL;
}