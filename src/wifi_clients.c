#include <homed/config.h>
#include <homed/commands.h>
#include <homed/wifi_clients.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct mac_address *add_macaddress(struct mac_address *head, const char *mac)
{
    struct mac_address *node = malloc(sizeof *node);
    if (!node) return head;

    strncpy(node->mac, mac, MAC_LEN);
    node->mac[MAC_LEN - 1] = '\0';
    node->next = head;
    return node;
}

int is_mac(const char *s)
{
    return strlen(s) == 17 &&
           isxdigit(s[0]) && isxdigit(s[1]) &&
           s[2] == ':' &&
           isxdigit(s[3]) && isxdigit(s[4]) &&
           s[5] == ':' &&
           isxdigit(s[6]) && isxdigit(s[7]) &&
           s[8] == ':' &&
           isxdigit(s[9]) && isxdigit(s[10]) &&
           s[11] == ':' &&
           isxdigit(s[12]) && isxdigit(s[13]) &&
           s[14] == ':' &&
           isxdigit(s[15]) && isxdigit(s[16]);
}

void free_macaddresses(struct mac_address *head)
{
    while (head) {
        struct mac_address *next = head->next;
        free(head);
        head = next;
    }
}

struct mac_address *wifi_clients(const void *arg)
{
    if (!arg)
    {
        fprintf(stderr, "wifi_clients: arg is NULL\n");
        return NULL;
    }

    const struct homed_config *cfg = arg;

    char cmd[512];
    ssh_cmd(cmd, sizeof(cmd), cfg);

    FILE *fp = popen(cmd, "r");
    if (!fp)
    {
        perror("popen");
        return NULL;
    }

    char buf[256];
    char current_iface[16] = "";

    struct mac_address *macs = NULL;

    while (fgets(buf, sizeof(buf), fp))
    {
        char mac[MAC_LEN];
        if (sscanf(buf, "IFACE=%15s assoclist %17s", current_iface, mac) == 2) {
            macs = add_macaddress(macs, mac);
        }
    }

    int rc = pclose(fp);
    if (rc == -1)
    {
        perror("pclose");
        return NULL;
    }

    return macs;
}