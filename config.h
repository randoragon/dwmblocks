static const Block blocks[] = {
    /* Icon, Command, Update, Interval, Update, Signal */

    // system updates
    {"📦", "cat ~/.cache/updatecount", 10, 0},

    {"", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0},

    {"", "cputemp", 1, 0},
};

// Block delimiter and right padding
static char delim[] = "  |  ";
static char rpad[] = "  ";
