//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
    /* Icon, Command, Update, Interval, Update, Signal */

    // system updates
    {"📦", "cat ~/.cache/updatecount", 10, 0},

    {"", "free -h | awk '/^Mem/ { print $3\"/\"$2 }' | sed s/i//g", 30, 0},

    {"", "cputemp", 1, 0},
};

//sets delimeter between status commands. NULL character ('\0') means no delimeter.
static char delim = ' ';
