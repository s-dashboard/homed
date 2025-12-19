#include <homed/config.h>
#include <homed/wifi_clients.h>
#include <homed/members.h>
#include <homed/timer.h>
#include "homed_paths.h"
#include <stdio.h>

int timer_call(void *arg)
{
    struct timer_ctx *ctx = arg;
    struct mac_address *macs = wifi_clients(ctx->cfg);

    if (macs == NULL)
    {
        return 1;
    }
    
    const char *owner;

    for (struct mac_address *n = macs; n; n = n->next)
    {
        owner = find_owner_from_mac(ctx->members, n->mac);

        // TODO: put mac and name into an "presence/homelist" that can be used 
        // to fire of events when the presence changes.
        if (owner) {
            // DEBUG
            printf("MAC %s → %s\n", n->mac, owner);
        }
    }

    free_macaddresses(macs);

    return 0;
}

int main(void)
{
    struct homed_config *cfg = homed_parse_config(HOMED_CONFIG_FILE);
    if (!cfg)
    {
        fprintf(stderr, "Failed to load main config\n");
        return 1;
    }

    struct member_owner *members = parse_members(HOMED_MEMBERS_FILE);
    if (!members)
    {
        fprintf(stderr, "Failed to load members config\n");
        return 1;
    }

    struct timer_ctx ctx = {
        .cfg = cfg,
        .members = members
    };

    timer(timer_call, &ctx, cfg->poll_interval);

    free_member_entries(members);

    return 0;
}