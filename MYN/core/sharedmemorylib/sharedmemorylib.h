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

/*

improvement :
    -check invalid mem pointer in function
    -check if locked is already done or not
*/

#ifndef _SHAREDMEMORYLIB_H
#define _SHAREDMEMORYLIB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __gnu_linux__

#define _SVID_SOURCE

#endif

#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include "../core_type.h"

#define MEMORYSIZE 2048
#define AREASTART 2

#define get_int(mem, ide) (((int *)mem->mem)[ide])
#define get_byte(mem, ide) (((uint8 *)mem->mem)[ide])

typedef struct
{
    void * mem;
    int areaCount;
    int semDescr;
    int semId;
}mymemory;

mymemory * createMemory(int shmDescr, int areaCount, int semDescr, int semId);
int openMemory(mymemory *  mem);
void cleanAreaMemories(mymemory *  mem);
void setState(mymemory *  mem, int state);
int getState(mymemory *  mem);
void setAction(mymemory *  mem, int action);
int getAction(mymemory *  mem);
int getAreaMemory(mymemory *  mem, int area_id, void ** area, int * size);
void * getNextAvailableAreaMemory(mymemory *  mem, int size);
int closeMemory(mymemory *  mem);
int freeMemory(mymemory *  mem);

int lockSem(int descr, int semid);
int unlockSem(int descr, int semid);


#endif
