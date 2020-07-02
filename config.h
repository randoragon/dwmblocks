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

    {"", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0},

    {"", "cputemp", 1, 0},
};

// Block delimiter and right padding
static char delim[] = "^c#FFFFFF^^b#000000^    ";
static char rpad[] = "^c#FFFFFF^^b#000000^  ";
