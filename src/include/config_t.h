#ifndef CONFIG_T_H
#define CONFIG_T_H

typedef struct CONFIG_T {
    char router_host[64];
    char router_user[32];
    char interfaces[128];
    int ssh_timeout;
    int poll_interval;
    int away_timeout;
} CONFIG_T;

#endif
