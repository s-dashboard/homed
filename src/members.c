#include <homed/wifi_clients.h>
#include <homed/members.h>
#include <homed/helpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static struct member_owner *add_entry(struct member_owner *head,
                                  const char *mac,
                                  const char *owner)
{
    struct member_owner *n = malloc(sizeof *n);
    if (!n)
        return head;

    strncpy(n->mac, mac, MAC_LEN);
    strncpy(n->name, owner, NAME_LEN);
    n->mac[MAC_LEN - 1] = '\0';
    n->name[NAME_LEN - 1] = '\0';

    n->next = head;
    return n;
}

void free_member_entries(struct member_owner *head)
{
    while (head) {
        struct member_owner *next = head->next;
        free(head);
        head = next;
    }
}

struct member_owner *parse_members(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    char line[LINE_LEN];
    char current_owner[NAME_LEN] = "";

    struct member_owner *list = NULL;

    while (fgets(line, sizeof line, fp)) {
        trim_newline(line);

        /* skip empty line */
        if (line[0] == '\0')
            continue;

        /* section header */
        if (line[0] == '[') {
            char *end = strchr(line, ']');
            if (end) {
                *end = '\0';
                strncpy(current_owner, line + 1, NAME_LEN);
                current_owner[NAME_LEN - 1] = '\0';
            }
            continue;
        }

        /* MAC line */
        if (current_owner[0] && is_mac(line)) {
            list = add_entry(list, line, current_owner);
        }
    }

    fclose(fp);
    return list;
}

const char *find_owner_from_mac(struct member_owner *list, const char *mac)
{
    for (; list; list = list->next) {
        if (strcasecmp(list->mac, mac) == 0)
            return list->name;
    }
    return NULL;
}
