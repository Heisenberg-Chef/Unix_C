/*Our own header,
to be included before all standard system headers */

#ifndef _APUE_H
#define _APUE_H

#if defined(SOLARIS)
#define _XOPEN_SOURCE 500   // Single UNIX Specification,Version 2 for Solaris 9
#define CMSG_LEN(x) _CMSG_DATA_ALIGN(sizeof(struct cmsghdr) + (x)) 
#elif !defined(BSD)
#define _XOPEN_SOURCE 600   // Single UNIX Specification,Version 3 */
#endif

#include <sys/types.h>  // Some System still require this
#include <sys/stat.h>
#include <sys/termios.h> // for winsize

#ifndef TIOCGWINSZ
#include <sys/ioctl.h>
#endif

#include <stdio.h>  // for convenience 
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define MAXLINE 4096

/*
*   Default file access permissions for new files;
*/

#define FILE_MODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

//  Default permissions for new files.

#define DIR_MODE    (FILE_MODE|S_IXUSR|S_IXGRP|S_IROTH)

typedef void Sigfunc(int); //for signal handlers

#if defined(SIG_IGN)&& !defined(SIG_ERR)
#define SIG_ERR ((Sigfunc *)-1)
#endif

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// Prototypes for our own functions.

char *path_alloc(int *); //Prog pathalloc
long open_max(void); // Prog openmax
void clr_fl(int,int); // Prog setfl
void set_fl(int,int); // Prog setfl
void pr_exit(int); // Prog prexit
void pr_mask(const char*); // Prog prmask
Sigfunc * signal_intr(int,Sigfunc); //Prog signal_intr_function

int tty_cbreak(int); // Prog raw
int tty_raw(int);   // Prog raw
int tty_reset(int);
void tty_atexit(void); //
#ifdef ECHO  // only if <termios.h> has been included
struct termios *tty_termios(void);
#endif

void sleep_us(unsigned int); //Ex sleepus
ssize_t readn(int,void*,size_t); // Prog IO
ssize_t writen(int,const void *,size_t);
void deamonize(const char*); // Prog daemoninit

int s_pipe(int *); // Prog streams_spipe sock_spipe
int recv_fd(int,ssize_t(*func)(int,const void *,size_t) // Progs recvfd_streams recvfd_sockets
int send





















#endif