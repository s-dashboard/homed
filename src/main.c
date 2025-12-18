#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_t.h"
#include "load_config.h"

int main(void) {
    CONFIG_T cfg = {0};

    if (load_config("/etc/homed/homed.conf", &cfg) != 0) {
        fprintf(stderr, "Failed to load config\n");
        return 1;
    }

    char cmd[512];
    ssh_cmd(cmd, sizeof(cmd), &cfg);

    fprintf(stderr, "\n--- SSH COMMAND BEGIN ---\n%s\n--- SSH COMMAND END ---\n\n", cmd);
    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("popen");
        return 1;
    }

    char buf[256];
    while (fgets(buf, sizeof(buf), fp)) {
        printf("ROUTER: %s", buf);
    }

    int rc = pclose(fp);
    if (rc != 0) {
        fprintf(stderr, "ssh command failed\n");
    }

    return 0;
}