#include <stdio.h>
#include "config_t.h"
#include "load_config.h"
#include "timer.h"
#include "wifi_clients.h"

int main(void) {
    CONFIG_T cfg = {0};

    if (load_config("/etc/homed/homed.conf", &cfg) != 0) {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    timer(wifi_clients, &cfg, cfg.poll_interval);

    return 0;
}