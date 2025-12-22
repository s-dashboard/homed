#ifndef WIFI_CLIENTS_H
#define WIFI_CLIENTS_H

enum
{
    MAC_LEN = 18
};

struct mac_address {
    char mac[MAC_LEN];
    struct mac_address *next;
};

struct mac_address *wifi_clients(const void *arg);
void free_macaddresses(struct mac_address *head);
int is_mac(const char *s);

#endif