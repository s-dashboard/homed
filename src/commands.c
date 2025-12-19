#include "config_t.h"
#include <stdlib.h>
#include <stdio.h>

int ssh_cmd(char *cmd, size_t cmdlen, const CONFIG_T *cfg)
{
    return snprintf(cmd, cmdlen,
                    "ssh -o BatchMode=yes "
                    "-o ConnectTimeout=%d "
                    "%s@%s "
                    "2>/dev/null "
                    "\"for i in %s; do wl -i \\$i assoclist; done\"",
                    cfg->ssh_timeout,
                    cfg->router_user,
                    cfg->router_host,
                    cfg->interfaces);
}
