#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_t.h"
#include "commands.h"

#define MAC_LEN 18

struct MAC_ADDRESS {
    char mac[MAC_LEN];
    struct MAC_ADDRESS *next;
};

struct MAC_ADDRESS *add_macaddress(struct MAC_ADDRESS *head, const char *mac)
{
    struct MAC_ADDRESS *node = malloc(sizeof *node);
    if (!node) return head;

    strncpy(node->mac, mac, MAC_LEN);
    node->mac[MAC_LEN - 1] = '\0';
    node->next = head;
    return node;
}


int wifi_clients(void *arg)
{

    if (!arg)
    {
        fprintf(stderr, "wifi_clients: arg is NULL\n");
        return 1;
    }

    CONFIG_T *cfg = arg;

    char cmd[512];
    ssh_cmd(cmd, sizeof(cmd), cfg);

    fprintf(stderr, "\n--- SSH COMMAND BEGIN ---\n%s\n--- SSH COMMAND END ---\n\n", cmd);

    FILE *fp = popen(cmd, "r");
    if (!fp)
    {
        perror("popen");
        return 1;
    }

    char buf[256];
    struct MAC_ADDRESS *macs = NULL;

    while (fgets(buf, sizeof(buf), fp))
    {
        char mac[MAC_LEN];
        if (sscanf(buf, "assoclist %17s", mac) == 1) {      
            macs = add_macaddress(macs, mac);
        }
    }

    int rc = pclose(fp);
    if (rc == -1)
    {
        perror("pclose");
        return 1; 
    }

    // TODO: Filter MAC-addresses against familymembers.conf and their specicied macadresses for their phones.    
    for (struct MAC_ADDRESS *n = macs; n; n = n->next) {
        printf("MAC: %s\n", n->mac);
    }

    return 0;
}