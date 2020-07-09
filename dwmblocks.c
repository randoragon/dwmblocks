#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<X11/Xlib.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<fcntl.h>
#define LENGTH(X)       (sizeof(X) / sizeof (X[0]))
#define CMDLENGTH		1024     /* this should match SLENGTH macro in dwm */
#define LINELENGTH      (CMDLENGTH*LENGTH(blocks))
#define SHM_NAME "/dwmstatus"

typedef struct {
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;
void dummysighandler(int num);
void sighandler(int num);
void getcmds(int time);
#ifndef __OpenBSD__
void getsigcmds(int signal);
void setupsignals();
void sighandler(int signum);
#endif
int getstatus(char *str, char *last);
void statusloop();
void termhandler(int signum);
void memwrite();
void updatedwm();
void cleanup();

#include "config.h"

static Display *dpy;
static int screen;
static Window root;
static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][LINELENGTH];
static int statusContinue = 1;
static void (*writestatus) () = memwrite;
static char *sharedmemory;
static int sharedmemoryfd;
static FILE *dwmbcpul;

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output, int last)
{
    output[0] = '\0';
	char *cmd = block->command;
    FILE *cmdf = popen(cmd,"r");
    if (!cmdf) {
        return;
    }

	int i;
    fgets(output, CMDLENGTH, cmdf);

    // trim newlines
    for (i = 0; i < CMDLENGTH && output[i] != '\n' && output[i] != '\0'; i++);
    output[i] = '\0';

    if (i)
        strcat(output, last ? rpad : delim);
	pclose(cmdf);
}

void getcmds(int time)
{
	const Block* current;
	for(int i = 0; i < LENGTH(blocks); i++) {
		current = blocks + i;
		if ((current->interval != 0 && time % current->interval == 0) || time == -1)
			getcmd(current,statusbar[i], i == LENGTH(blocks) - 1);
	}
}

#ifndef __OpenBSD__
void getsigcmds(int signal)
{
	const Block *current;
	for (int i = 0; i < LENGTH(blocks); i++)
	{
		current = blocks + i;
		if (current->signal == signal)
			getcmd(current,statusbar[i], i == LENGTH(blocks) - 1);
	}
}

void setupsignals()
{
    /* initialize all real time signals with dummy handler */
    for(int i = SIGRTMIN; i <= SIGRTMAX; i++)
        signal(i, dummysighandler);

    for(int i = 0; i < LENGTH(blocks); i++)
	{
        if (blocks[i].signal > 0)
			signal(SIGRTMIN+blocks[i].signal, sighandler);
	}
}
#endif

int getstatus(char *str, char *last)
{
	strcpy(last, str);
    strcpy(str, lpad);
	for(int i = 0; i < LENGTH(blocks); i++)
		strcat(str, statusbar[i]);
	return strcmp(str, last);//0 if they are the same
}

void memwrite()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only set root if text has changed.
		return;
    strcpy(sharedmemory, statusstr[0]);

    updatedwm();
}

void updatedwm()
{
    // Send a fake SIGUSR1 signal to dwm to update status text
    Display *d = XOpenDisplay(NULL);
    if (d) {
        dpy = d;
    }
    screen = DefaultScreen(dpy);
    root = RootWindow(dpy, screen);
    XStoreName(dpy, root, "fsignal:10");
    XCloseDisplay(dpy);
}

void pstdout()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only write out if text has changed.
		return;
	printf("%s\n",statusstr[0]);
	fflush(stdout);
}


void statusloop()
{
#ifndef __OpenBSD__
	setupsignals();
#endif
	int i = 0;
	getcmds(-1);
	while(statusContinue)
	{
		getcmds(i);
		writestatus();
		sleep(1.0);
		i++;
	}
}

#ifndef __OpenBSD__
/* this signal handler should do nothing */
void dummysighandler(int signum)
{
    return;
}
#endif

#ifndef __OpenBSD__
void sighandler(int signum)
{
	getsigcmds(signum-SIGRTMIN);
	writestatus();
}
#endif

void termhandler(int signum)
{
    strcpy(sharedmemory, "^f5^^c#FFFFFF^dwmblocks is offline^f5^");
    updatedwm();

    shm_unlink(sharedmemory);

	statusContinue = 0;
    cleanup();
	exit(0);
}

void cleanup()
{
    if (dwmbcpul) {
        pclose(dwmbcpul);
    }
}

int main(int argc, char** argv)
{
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);

    /* initialize shared memory */
    sharedmemoryfd = shm_open(SHM_NAME, O_CREAT|O_RDWR, S_IRWXU|S_IRWXG);
    if (sharedmemoryfd < 0) {
        perror("dwmblocks: failed to open shared memory");
        return EXIT_FAILURE;
    }
    ftruncate(sharedmemoryfd, LINELENGTH);
    sharedmemory = (char*)mmap(NULL, LINELENGTH, PROT_READ|PROT_WRITE, MAP_SHARED, sharedmemoryfd, 0);
    if (sharedmemory == NULL) {
        fprintf(stderr, "dwmblocks: failed to run mmap");
        return EXIT_FAILURE;
    }

    /* spawn subprocesses */
    FILE *dwmbcpul;
    dwmbcpul = popen(DWMBCPUL_CMD, "r");

	statusloop();
    cleanup();
}
