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

#ifndef _DELAYEDLIB_H
#define _DELAYEDLIB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __gnu_linux__

#define _BSD_SOURCE
#define _ISOC99_SOURCE
#define _POSIX_SOURCE
#include <sys/time.h>
#include <sys/types.h>

#endif

#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include "../core_type.h"

#define DELAYED_PACKET_IN_FILE_LIMIT 2000 /*it will be a non sense to set a smallest limit here than the memory limit*/
#define DELAYED_MAX_FILE_COUNT 50
#define DELAYED_MAX_PACKET_IN_MEMORY 500
#define DELAYED_THRESHOLD 10000 /*en microseconde*/

#define DELAYED_FLAG_INIT            1
#define DELAYED_FLAG_PAUSE           2
#define DELAYED_FLAG_ENABLE          4
#define DELAYED_FLAG_DATA_ON_FILE    8
#define DELAYED_FLAG_DONT_DELAY_NEXT 16

typedef struct time_packet
{
    struct timeval T;
    struct time_packet * next; /*next in time*/
    unsigned int size;
    uint8 * datas;
}time_packet;

/*
time_t         tv_sec      seconds
suseconds_t    tv_usec     microseconds 1 seconds = 1.000.000 microseconds
*/

/*DELAY SYSTEM VARS*/
struct timeval T, Tprime, ref_system, ref_packet;
time_packet * head_buffered_packet_input, * queue_buffered_packet_input,* head_buffered_packet_output, * queue_buffered_packet_output;
uint8 delay_factor, delay_flags;

/*DELAY FILE VAR*/
unsigned int file_count, packet_in_last_file, last_file_id,input_buffer_size,packet_read_from_last_file, input_offset;
unsigned int output_offset;
unsigned int output_buffer_size; /*debug vars*/
int input_file_descriptor, output_file_descriptor;

void setDelay(sint8 value);
void delay_init();
int delay_updateTime();
int delay_needToDelay(struct timeval * packet_time_T);
time_packet * delay_allocateTemporalPaquet(unsigned int size,struct timeval * t);
int delay_sendDelayedPacket(int socket, int * send_the_paquet);
void delay_flush();

int bufferToFile(time_packet * from, int limit);
int fileToBuffer();

#endif