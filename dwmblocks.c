#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<X11/Xlib.h>
#define LENGTH(X)               (sizeof(X) / sizeof (X[0]))
#define CMDLENGTH		50

typedef struct {
	char* icon;
	char* command;
	unsigned int interval;
	unsigned int signal;
} Block;
void dummysighandler(int num);
void sighandler(int num);
void buttonhandler(int sig, siginfo_t *si, void *ucontext);
void getcmds(int time);
#ifndef __OpenBSD__
void getsigcmds(int signal);
void setupsignals();
void sighandler(int signum);
#endif
int getstatus(char *str, char *last);
void setroot();
void statusloop();
void termhandler(int signum);


#include "config.h"

static Display *dpy;
static int screen;
static Window root;
static char statusbar[LENGTH(blocks)][CMDLENGTH] = {0};
static char statusstr[2][256];
static char button[] = "\0";
static int statusContinue = 1;
static void (*writestatus) () = setroot;

//opens process *cmd and stores output in *output
void getcmd(const Block *block, char *output, int last)
{
	if (block->signal) {
		output[0] = block->signal;
		output++;
	}
	strcpy(output, block->icon);
	char *cmd = block->command;
	FILE *cmdf;
	if (*button) {
		setenv("BUTTON", button, 1);
		cmdf = popen(cmd,"r");
		*button = '\0';
		unsetenv("BUTTON");
	} else {
		cmdf = popen(cmd,"r");
	}
	if (!cmdf)
		return;
	char c;
	int i = strlen(block->icon);
	fgets(output+i, CMDLENGTH-i, cmdf);
	i = strlen(output);
    if (--i) {
        if (!last && strlen(delim)) {
            strcpy(output + i, delim);
            i += strlen(delim);
        } else if (last && strlen(rpad)) {
            strcpy(output + i, rpad);
            i += strlen(rpad);
        }
    }
    output[i++] = ' ';
	output[i] = '\0';
	pclose(cmdf);
}

void getcmds(int time)
{
	const Block* current;
	for(int i = 0; i < LENGTH(blocks); i++)
	{
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
    struct sigaction sa;
    for(int i = SIGRTMIN; i <= SIGRTMAX; i++)
        signal(i, dummysighandler);

	for(int i = 0; i < LENGTH(blocks); i++)
	{
        if (blocks[i].signal > 0) {
			signal(SIGRTMIN+blocks[i].signal, sighandler);
 			sigaddset(&sa.sa_mask, SIGRTMIN+blocks[i].signal); // ignore signal when handling SIGUSR1
        }
	}

	sa.sa_sigaction = buttonhandler;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
}
#endif

int getstatus(char *str, char *last)
{
	strcpy(last, str);
	str[0] = '\0';
	for(int i = 0; i < LENGTH(blocks); i++)
		strcat(str, statusbar[i]);
	str[strlen(str)-1] = '\0';
	return strcmp(str, last);//0 if they are the same
}

void setroot()
{
	if (!getstatus(statusstr[0], statusstr[1]))//Only set root if text has changed.
		return;
	Display *d = XOpenDisplay(NULL);
	if (d) {
		dpy = d;
	}
	screen = DefaultScreen(dpy);
	root = RootWindow(dpy, screen);
	XStoreName(dpy, root, statusstr[0]);
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

void buttonhandler(int sig, siginfo_t *si, void *ucontext)
{
	*button = '0' + si->si_value.sival_int & 0xff;
	getsigcmds(si->si_value.sival_int >> 8);
	writestatus();
}
#endif

void termhandler(int signum)
{
	statusContinue = 0;
	exit(0);
}

int main(int argc, char** argv)
{
	signal(SIGTERM, termhandler);
	signal(SIGINT, termhandler);
	statusloop();
}
