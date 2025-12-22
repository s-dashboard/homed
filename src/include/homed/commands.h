#ifndef COMMANDS_H
#define COMMANDS_H
#include <stddef.h>
#include <homed/config.h>

int ssh_cmd(char *cmd, size_t cmdlen, const struct homed_config *cfg);

#endif
