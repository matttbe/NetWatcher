/*
                    GNU GENERAL PUBLIC LICENSE
                       Version 3, 29 June 2007

 Copyright (C) 2007 Free Software Foundation, Inc. <http://fsf.org/>
 Everyone is permitted to copy and distribute verbatim copies
 of this license document, but changing it is not allowed.

                            Preamble

  The GNU General Public License is a free, copyleft license for
software and other kinds of works.

  The licenses for most software and other practical works are designed
to take away your freedom to share and change the works.  By contrast,
the GNU General Public License is intended to guarantee your freedom to
share and change all versions of a program--to make sure it remains free
software for all its users.  We, the Free Software Foundation, use the
GNU General Public License for most of our software; it applies also to
any other work released this way by its authors.  You can apply it to
your programs, too.

  When we speak of free software, we are referring to freedom, not
price.  Our General Public Licenses are designed to make sure that you
have the freedom to distribute copies of free software (and charge for
them if you wish), that you receive source code or can get it if you
want it, that you can change the software or use pieces of it in new
free programs, and that you know you can do these things.

  To protect your rights, we need to prevent others from denying you
these rights or asking you to surrender the rights.  Therefore, you have
certain responsibilities if you distribute copies of the software, or if
you modify it: responsibilities to respect the freedom of others.
*/

#ifndef _CONTROLLIB_H
#define _CONTROLLIB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __gnu_linux__

#define _BSD_SOURCE
#define _SVID_SOURCE
#define _POSIX_SOURCE
#include <sys/types.h>

#endif

#ifdef HAVE_PCAP_H
#include <pcap.h>
#endif

#ifdef HAVE_PCAP_PCAP_H
#include <pcap/pcap.h>
#endif

#if !(defined(HAVE_PCAP_H) || defined(HAVE_PCAP_PCAP_H))
#include <pcap.h>
#include <pcap/pcap.h>
#endif

#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <signal.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/stat.h>
#include "../core_type.h"
#include "../command.h"

#include "../collectorlib/collectorlib.h"
#include "../utillib/utillib.h"
#include "../sharedmemorylib/sharedmemorylib.h"

char errbuf[PCAP_ERRBUF_SIZE];

typedef struct
{
    sint32 size;
    char * name;
} entry;

#define ARG_SET 0
#define ARG_GET 1

typedef struct
{
    char type;
    void * values;
    int size;
} my_args;

extern int from_collector, to_collector, dispatch_id;
extern mymemory * mem;

extern char * home_directory;

int sendEntries(int socket);
int getProtocolList(int socket);
int clearProtocolList(int socket);
int setLengthProtocolList(int socket);
int selectCaptureDevice(int socket, int command);
/*int disableCaptureDevice(int socket);*/
int selectCaptureFile(int socket);
int setSpeed(int socket);
/*int parseFile(int socket);*/
/*int startCapture(int socket);*/
int stopCapture(int socket);
/*int stopAllCapture(int socket);*/
int listFiles(int socket);
int startRecord(int socket);
/*int stopRecord(int socket);*/
int setCaptureMode(int mode);

int setFilter(int socket, int command);

int directTransmit(int socket, int command);

int sendCommandToDispatch(int command, int argc_set, int argc_get, ...);

int file_goto(int descriptor, int command);

int get_state(int socket);

int disable_device(int socket);

#endif
