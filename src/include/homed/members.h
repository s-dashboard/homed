
#ifndef MEMBERS_H
#define MEMBERS_H

#include <homed/wifi_clients.h>
#include <homed/bt.h>

enum 
{
    NAME_LEN = 64,
    LINE_LEN = 256,
    PATH_LEN = 512
};

struct phone {
    char mac[MAC_LEN];
    char addr[BT_ADDR_LEN];
    struct phone *next;
};

struct member_owner {
    char name[NAME_LEN];
    char sound_in[PATH_LEN];
    char sound_out[PATH_LEN];
    struct phone *phones;
    struct member_owner *next;
};

const struct member_owner *find_owner_from_mac(const struct member_owner *list, const char *mac);
const struct member_owner *find_owner_from_device(const struct member_owner *list, const char *addr);
struct member_owner *parse_members(const char *path);
void free_members(struct member_owner *head);

#endif