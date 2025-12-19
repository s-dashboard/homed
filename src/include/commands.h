#ifndef COMMANDS_H
#define COMMANDS_H
#include "config_t.h"
#include <stddef.h>

int ssh_cmd(char *cmd, size_t cmdlen, const CONFIG_T *cfg);

#endif
