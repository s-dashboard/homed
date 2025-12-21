#ifndef PRESENCE_H
#define PRESENCE_H

#include <time.h>
#include <stdbool.h>

enum presence_state {
    PRESENCE_ABSENT = 0,
    PRESENCE_PRESENT = 1
};

struct presence_entry {
    const struct member_owner *member;   /* non-owning reference */
    enum presence_state current;
    enum presence_state previous;
    time_t last_seen;               /* last time a phone was detected */
    struct presence_entry *next;
};

struct presence_list {
    struct presence_entry *head;
};

struct presence_list *presence_create(const struct member_owner *members);
void presence_mark_seen(struct presence_list *pl, const struct member_owner *m, time_t now);
void presence_finalize_tick(struct presence_list *pl, time_t now, time_t timeout);
void presence_fire_events(const struct presence_list *pl);
void presence_reset_tick(struct presence_list *pl);
void presence_free(struct presence_list *pl);

#endif