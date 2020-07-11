/* IMPORTANT
 *
 * CMDLENGTH holds the number of bytes allowed per block output.
 * Be sure it is always well above your safe output length (this includes
 * delim, lpad and rpad).
 *
 * SHM_SIZE holds the number of bytes allowed per statusstr.
 * Be sure it's safely above the maximum statusstr length.
 * This value is also stored in a macro with the same name
 * in dwm.c, be sure that the two are always in sync.
 */

#define CMDLENGTH		300     /* max size for module output (bytes) */
#define SHM_SIZE        1500    /* max size for the entire status string */
#define DWMBCPUL_CMD    "~/.scripts/dwmblocks/cpuload"

static const Block blocks[] = {
    /* Command, Update Interval, Update Signal */

    // system updates
    {"cat ~/.cache/dwmbupdates", 0, 4},

    {"cat ~/.cache/newsboat_notify", 0, 5},

    {"~/.scripts/dwmblocks/mpd", 1, 6},

    {"~/.scripts/dwmblocks/storage", 10, 0},

    {"~/.scripts/dwmblocks/memory", 1, 0},

    {"cat ~/.cache/dwmbcpul", 1, 0},

    {"~/.scripts/dwmblocks/cputemp", 1, 0},

    {"~/.scripts/dwmblocks/network", 1, 0},
    
    {"~/.scripts/dwmblocks/pulseaudio", 5, 7},

    {"~/.scripts/dwmblocks/date", 1, 0},

    {"~/.scripts/dwmblocks/time", 1, 0},

    {"~/.scripts/dwmblocks/power", 3, 0},
};

// Block delimiter and paddings
static char delim[] = "^c#313131^^f4^^r0,0,1,19^^f5^^d^";
static char lpad[] = "^f-1^^c#313131^^r0,0,1,19^^f5^^d^";
static char rpad[] = "^f5^";
