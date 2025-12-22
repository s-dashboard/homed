#include <stdio.h>
#include <stdlib.h>

struct bt_device *bt_devices(const void *arg) {
    printf("DEBUG: Scan BT devices\n");

    if (!arg)
    {
        fprintf(stderr, "bt_devices: arg is NULL\n");
        return NULL;
    }

    const struct homed_config *cfg = arg;
    (void)cfg; // mark as unused for the moment, remeber to remove. :D

    // TODO 
    // 1. connect to BT-dongle (perhaps outside timer) humm ... yes yes...
    // 2. scan and check advertising reports from devices
    
    return NULL;
}