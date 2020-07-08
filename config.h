/* BE WARNED
 * dwmblocks.c defines a CMDLENGTH macro which holds the number of characters
 * allowed per block output. Be sure CMDLENGTH is always well above your safe
 * output length (this includes delim and rpad), because if you overflow, dwm
 * will very likely crash instantly.
 */

static const Block blocks[] = {
    /* Command, Update Interval, Update Signal */

    // system updates
    {"cat ~/.cache/updatecount", 0, 4},

    {"cat ~/.cache/newsboat_notify", 0, 5},

    {"~/.scripts/dwmblocks/mpd", 1, 6},

    {"~/.scripts/dwmblocks/storage", 10, 0},

    {"~/.scripts/dwmblocks/memory", 1, 0},

    {"~/.scripts/dwmblocks/cputemp", 1, 0},

    {"~/.scripts/dwmblocks/network", 1, 0},
    
    {"~/.scripts/dwmblocks/pulseaudio", 5, 7},

    {"~/.scripts/dwmblocks/date", 1, 0},

    {"~/.scripts/dwmblocks/time", 1, 0},
};

// Block delimiter and paddings
static char delim[] = "^c#313131^^f4^^r0,0,1,19^^f5^^d^";
static char lpad[] = "^f-1^^c#313131^^r0,0,1,19^^f1^^d^";
static char rpad[] = "^f5^";
