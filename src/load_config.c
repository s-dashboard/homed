#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "config_t.h"

static void trim(char *s) {
    char *end;

    // trim leading
    while (isspace((unsigned char)*s)) s++;

    // trim trailing
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end))
        *end-- = '\0';
}

int load_config(const char *path, CONFIG_T *cfg) {
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        char key[64], val[128];
        if (sscanf(line, " %63[^=]= %127[^\n]", key, val) != 2)
            continue;

        // trim config-keys and values.
        trim(key); 
        trim(val);

        if (strcmp(key, "router_host") == 0)
            strncpy(cfg->router_host, val, sizeof(cfg->router_host));
        else if (strcmp(key, "router_user") == 0)
            strncpy(cfg->router_user, val, sizeof(cfg->router_user));
        else if (strcmp(key, "router_interfaces") == 0)
            strncpy(cfg->interfaces, val, sizeof(cfg->interfaces));
        else if (strcmp(key, "ssh_timeout") == 0)
            cfg->ssh_timeout = atoi(val);
        else if (strcmp(key, "away_timeout") == 0)
            cfg->away_timeout = atoi(val);
        else if (strcmp(key, "poll_interval") == 0)
            cfg->poll_interval = atoi(val);
    }

    fclose(f);
    return 0;
}

int ssh_cmd(char *cmd, size_t cmdlen, const CONFIG_T *cfg)
{
    return snprintf(cmd, cmdlen,
                    "ssh -o BatchMode=yes "
                    "-o ConnectTimeout=%d "
                    "%s@%s "
                    "\"for i in %s; do wl -i \\$i assoclist; done\"",
                    cfg->ssh_timeout,
                    cfg->router_user,
                    cfg->router_host,
                    cfg->interfaces);
}
