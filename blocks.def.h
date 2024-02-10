//Modify this file to change what commands output to your statusbar, and recompile using the make command.
static const Block blocks[] = {
	/*Icon         Command                              Update Interval  Update Signal*/
	{"",           "~/.scripts/dwmblocks/volume",       5,               7},
	{"",           "~/.scripts/dwmblocks/storage",      10,              0},
	{"",           "~/.scripts/dwmblocks/gpg_expire",   3600,            0},
	{"",           "date +'%a %m/%d  %H:%M:%S'",        1,               0},
	{"",           "~/.scripts/dwmblocks/battery",      10,              0},
};

//sets delimiter between status commands. NULL character ('\0') means no delimiter.
static char delim[] = "  │  ";
static unsigned int delimLen = 7;
