#include <homed/wifi_clients.h>
#include <homed/members.h>
#include <homed/helpers.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void free_members(struct member_owner *head)
{
    while (head) {
        struct member_owner*next = head->next;

        struct phone *p = head->phones;
        while (p) {
            struct phone *pn = p->next;
            free(p);
            p = pn;
        }

        free(head);
        head = next;
    }   
}

static void add_phone(struct member_owner *m, const char *mac)
{
    struct phone *p = malloc(sizeof *p);
    if (!p)
        return;

    strncpy(p->mac, mac, MAC_LEN);
    p->mac[MAC_LEN - 1] = '\0';

    p->next = m->phones;
    m->phones = p;
}

struct member_owner *parse_members(const char *path)
{
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        return NULL;
    }

    char line[LINE_LEN];
    struct member_owner *list = NULL;
    struct member_owner *current = NULL;

    while (fgets(line, sizeof line, fp)) {
        trim_newline(line);

        if (line[0] == '\0')
            continue;

        /* section header */
        if (line[0] == '[') {
            char *end = strchr(line, ']');
            if (!end)
                continue;

            *end = '\0';

            current = calloc(1, sizeof *current);
            if (!current)
                break;

            strncpy(current->name, line + 1, NAME_LEN);
            current->name[NAME_LEN - 1] = '\0';

            current->next = list;
            list = current;
            continue;
        }

        if (!current)
            continue;

        /* phones= */
        if (strncmp(line, "phones=", 7) == 0) {
            char *p = line + 7;
            char *tok;

            while ((tok = strtok(p, " "))) {
                p = NULL;
                if (is_mac(tok))
                    add_phone(current, tok);
            }
            continue;
        }

        /* in= */
        if (strncmp(line, "in=", 3) == 0) {
            strncpy(current->sound_in, line + 3, PATH_LEN);
            current->sound_in[PATH_LEN - 1] = '\0';
            continue;
        }

        /* out= */
        if (strncmp(line, "out=", 4) == 0) {
            strncpy(current->sound_out, line + 4, PATH_LEN);
            current->sound_out[PATH_LEN - 1] = '\0';
            continue;
        }
    }

    fclose(fp);

    return list;
}


const struct member_owner *find_owner_from_mac(
    const struct member_owner *list,
    const char *mac)
{
    for (; list; list = list->next) {
        for (struct phone *p = list->phones; p; p = p->next) {
            if (strcasecmp(p->mac, mac) == 0)
                return list;
        }
    }
    return NULL;
}

const struct member_owner *find_owner_from_device(
    const struct member_owner *list, 
    const char *addr)
{
    for (; list; list = list->next) {
        for (struct phone *p = list->phones; p; p = p->next) {
            if (strcasecmp(p->addr, addr) == 0)
                return list;
        }
    }
    return NULL;
}