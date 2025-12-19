
#ifndef MEMBERS_H
#define MEMBERS_H

enum 
{
    NAME_LEN = 64,
    LINE_LEN = 256
};

struct member_owner {
    char mac[MAC_LEN];
    char name[NAME_LEN];
    struct member_owner *next;
};

const char *find_owner_from_mac(struct member_owner *list, const char *mac);
struct member_owner *parse_members(const char *path);
void free_member_entries(struct member_owner *head);

#endif