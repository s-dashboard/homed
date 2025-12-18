#ifndef LOAD_CONFIG_H
#define LOAD_CONFIG_H
#include "config_t.h"
#include <stddef.h>

int load_config(const char *path, CONFIG_T *cfg);
int ssh_cmd(char *cmd, size_t cmdlen, const CONFIG_T *cfg);

#endif