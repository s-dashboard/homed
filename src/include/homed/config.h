#ifndef HOMED_CONFIG_H
#define HOMED_CONFIG_H

struct homed_config {
    char router_host[64];
    char router_user[32];
    char interfaces[128];
    int ssh_timeout;
    int poll_interval;
    int away_timeout;
};

struct homed_config *homed_parse_config(const char *path);
void homed_free_config(struct homed_config *cfg);

#endif