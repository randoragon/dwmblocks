/* BE WARNED
 * dwmblocks.c defines a CMDLENGTH macro which holds the number of characters
 * allowed per block output. Be sure CMDLENGTH is always well above your safe
 * output length (this includes delim and rpad), because if you overflow, dwm
 * will very likely crash instantly.
 */

static const Block blocks[] = {
    /* Icon, Command, Update, Interval, Update, Signal */

    // system updates
    {"📦", "cat ~/.cache/updatecount", 10, 0},

    {"", "memory", 1, 0},

    {"", "cputemp", 1, 0},
};

// Block delimiter and right padding
static char delim[] = "^c#313131^^f4^^r0,0,1,20^^f4^^c#FFFFFF^^b#000000^";
static char rpad[] = "^c#FFFFFF^^b#000000^^f5^";
