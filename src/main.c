#include <homed/config.h>
#include <homed/wifi_clients.h>
#include <homed/members.h>
#include <homed/presence.h>
#include <homed/timer.h>
#include <homed/bt.h>
#include "homed_paths.h"
#include <stdio.h>
#include <time.h>

int presence_scan_bt(
    const struct homed_config *cfg,
    const struct member_owner *members,
    struct presence_list *presence,
    time_t now
) {
    struct bt_device *devices = bt_devices(cfg);
    if(devices == NULL) {
        return 1;
    }

    // mark seen
    for(struct bt_device *n = devices; n; n = n->next) {
        const struct member_owner *m = find_owner_from_device(members, n->addr);
        if (m)
        {
            presence_mark_seen(presence, m, now);
        }
    }

    return 0;
}

int presence_scan_wifi(
    const struct homed_config *cfg,
    const struct member_owner *members,
    struct presence_list *presence,
    time_t now)
{

    // Scan MACs:
    struct mac_address *macs = wifi_clients(cfg);
    if (macs == NULL)
    {
        return 1;
    }

    // mark seen
    for (struct mac_address *n = macs; n; n = n->next)
    {
        const struct member_owner *m = find_owner_from_mac(members, n->mac);
        if (m)
        {
            presence_mark_seen(presence, m, now);
        }
    }

    free_macaddresses(macs);

    return 0;
}

int timer_call(void *arg)
{
    struct timer_ctx *ctx = arg;
    time_t now = time(NULL);

    presence_reset_tick(ctx->presence);

    // Try with bluetooth scanning instead, 
    // router seems to not list some phone even though they are connected 
    // and uses the network. Weird .. but true... 
    if (presence_scan_bt(
            ctx->cfg,
            ctx->members,
            ctx->presence,
            now) < 0)
    {

        // So WIFI-scanning seems a bit unstable -- it can stay as an back up or so.
        if (presence_scan_wifi(
                ctx->cfg,
                ctx->members,
                ctx->presence,
                now) < 0)
        {
            return -1;
        }
    }

    presence_finalize_tick(
        ctx->presence,
        now,
        ctx->cfg->away_timeout
    );
    
    presence_fire_events(ctx->presence);

    return 0;
}

int main(void)
{
    printf("DEBUG: Parse config file\n");

    struct homed_config *cfg = homed_parse_config(HOMED_CONFIG_FILE);
    if (!cfg)
    {
        fprintf(stderr, "Failed to load main config\n");
        return 1;
    }

    printf("DEBUG: Parse members\n");
    struct member_owner *members = parse_members(HOMED_MEMBERS_FILE);
    if (!members)
    {
        fprintf(stderr, "Failed to load members config\n");
        return 1;
    }

    printf("DEBUG: Create presence list\n");
    struct presence_list *pl = presence_create(members);
    if (!pl) {
        fprintf(stderr, "Failed to create presence list\n");
        free_members(members);
        return 1;
    }

    struct timer_ctx ctx = {
        .cfg = cfg,
        .members = members,
        .presence = pl
    };

    printf("DEBUG: Start timer\n");
    timer(timer_call, &ctx, cfg->poll_interval);

    // free memory and shutdown.
    presence_free(pl);
    free_members(members);
    homed_free_config(cfg);

    return 0;
}