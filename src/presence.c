#include <homed/presence.h>
#include <homed/members.h>
#include <homed/sound.h>
#include <stdio.h>
#include <stdlib.h>

struct presence_list *presence_create(const struct member_owner *members)
{
    struct presence_list *pl = calloc(1, sizeof *pl);
    if (!pl)
        return NULL;

    for (const struct member_owner *m = members; m; m = m->next) {
        struct presence_entry *e = calloc(1, sizeof *e);
        if (!e)
            continue;

        e->member = m;
        e->current = PRESENCE_ABSENT;
        e->previous = PRESENCE_ABSENT;
        e->next = pl->head;
        pl->head = e;
    }

    return pl;
}

void presence_mark_seen(
    struct presence_list *pl,
    const struct member_owner *m,
    time_t now)
{
    for (struct presence_entry *e = pl->head; e; e = e->next) {
        if (e->member == m) {
            e->current = PRESENCE_PRESENT;
            e->last_seen = now;

            char buf[64];
            struct tm tm;
            localtime_r(&e->last_seen, &tm);
            strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", &tm);

            printf("DEBUG: mark seen %s at %s\n", m->name, buf);
            return;
        }
    }
}

void presence_reset_tick(struct presence_list *pl)
{
    if (!pl)
        return;

    for (struct presence_entry *e = pl->head; e; e = e->next) {
        e->previous = e->current;        // ← preserve history
        e->current = PRESENCE_ABSENT;    // ← reset for new tick
    }
}

void presence_finalize_tick(struct presence_list *pl, time_t now, time_t timeout)
{
    for (struct presence_entry *e = pl->head; e; e = e->next)
    {
        if (e->current == PRESENCE_PRESENT &&
            now - e->last_seen > timeout)
        {
            e->current = PRESENCE_ABSENT;
        }
    }
}

void presence_fire_events(const struct presence_list *pl)
{
    printf("DEBUG: fire events\n");
    
    for (const struct presence_entry *e = pl->head; e; e = e->next) {

        printf("DEBUG: current: %d prev: %d => %s\n", e->current, e->previous, e->member->name);
        
        if (e->current == PRESENCE_PRESENT &&
            e->previous == PRESENCE_ABSENT)
        {
            play_sound(e->member->sound_in);
        }

        if (e->current == PRESENCE_ABSENT &&
            e->previous == PRESENCE_PRESENT)
        {
            play_sound(e->member->sound_out);
        }
    }
}

void presence_free(struct presence_list *pl)
{
    if (!pl)
        return;

    struct presence_entry *e = pl->head;
    while (e) {
        struct presence_entry *next = e->next;
        free(e);
        e = next;
    }
    free(pl);
}




