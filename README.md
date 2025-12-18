# homed
homed is a lightweight daemon that gives your home a voice. It detects presence on the local network and announces simple, human-friendly house status changes.

## homed.config
The used router has support for SSH, so practically I just log in and fetch the
currently active MAC-addresses on the WIFI (2.4Ghz and 5Ghz)


router_host = 

router_user = 

router_interfaces = eth5,eth6

ssh_timeout = 5

poll_interval = 30

away_timeout = 180



## Pseudo code
for i in eth5 eth6; do
    wl -i $i assoclist addto wl_assoclist
done

if phone_mac in wl_assoclist:
    person = HOME
else:
    person = AWAY
