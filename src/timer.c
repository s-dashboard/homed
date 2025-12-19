#include <unistd.h>

void timer(int (*fn)(void *), void *arg, int seconds) {
    while(1) {
        int error = fn(arg);
        if(error != 0) break;
        
        sleep(seconds); 
    }
}