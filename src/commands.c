#include <stdlib.h>
#include <stdio.h>
#include <homed/config.h>

int ssh_cmd(char *cmd, size_t cmdlen, struct homed_config *cfg)
{
    return snprintf(cmd, cmdlen,
                    "ssh -o BatchMode=yes "
                    "-o ConnectTimeout=%d "
                    "%s@%s "
                    "2>/dev/null "
                    "\"for i in %s; do "
                    "wl -i \\$i assoclist | sed \\\"s/^/IFACE=\\$i /\\\"; "
                    "done\"",
                    cfg->ssh_timeout,
                    cfg->router_user,
                    cfg->router_host,
                    cfg->interfaces);
}
