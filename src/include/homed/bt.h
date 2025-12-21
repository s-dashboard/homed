#ifndef BT_H
#define BT_H

enum {
    BT_ADDR_LEN = 18
};

struct bt_device {
    // fix addr - need to test to find human readable from BT-advertising data (phone name or likewise)
    char addr[BT_ADDR_LEN];
    struct bt_device *next;
    
};

struct bt_device *bt_devices(void *arg);

#endif