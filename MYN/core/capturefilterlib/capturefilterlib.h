/*
 * Copyright (c) 1992, 1993, 1994, 1995, 1996
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the University of California,
 * Lawrence Berkeley Laboratory and its contributors. (4) Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */


#ifndef _CAPTUREFILTERLIB_H
#define _CAPTUREFILTERLIB_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef __gnu_linux__

#define _BSD_SOURCE
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

#include "../core_type.h"
#include "../wait_communication.h"
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#ifndef LBL_ALIGN
#ifndef WIN32
#include <netinet/in.h>
#endif

#define EXTRACT_SHORT(p)	((u_short)ntohs(*(u_short *)p))
#define EXTRACT_LONG(p)		(ntohl(*(uint32 *)p))
#else
#define EXTRACT_SHORT(p)\
	((u_short)\
		((u_short)*((u_char *)p+0)<<8|\
		 (u_short)*((u_char *)p+1)<<0))
#define EXTRACT_LONG(p)\
		((uint32)*((u_char *)p+0)<<24|\
		 (uint32)*((u_char *)p+1)<<16|\
		 (uint32)*((u_char *)p+2)<<8|\
		 (uint32)*((u_char *)p+3)<<0)
#endif

struct filter_node
{
    struct filter_item * item;
    struct filter_node * next;
};

struct filter_item
{
    u_short	code;
    uint32 k;
      
    struct filter_node * next_child_t; /*liste chainée des enfants*/
        /*Dans le cas d'une instruction NON CONDITIONNELLE, le next_child_t contiendra l'instruction suivante*/
        /*Dans le cas d'une instruction TERMINALE, le next_child_t contiendra les parents a partir du second*/
    struct filter_node * next_child_f; /*liste chainée des enfants pour un noeud NON CONDITIONNELLE uniquement*/
    struct filter_item * parent; /*contient uniquement un et un seul parent*/
        /*Dans le cas d'une instruction NON CONDITIONNELLE, le parent contiendra l'unique parent*/
        /*Dans le cas d'une instruction TERMINALE, le parent contiendra le premier parent*/
};

struct filter_end_node
{
    struct filter_node * next_end_node;
    int control_pipe;
    int port_id;
    struct filter_end_node * next;
};

struct breakpoint
{
    uint32 A, X;
	int k;
	sint32 mem[BPF_MEMWORDS];
    struct filter_node * next_instruct;
};

struct master_filter
{
    struct filter_node * filter_first_node;
    struct filter_end_node * end_node;
    
    struct breakpoint * breakpoint_stack;
    int current_breakpoint;
    unsigned int max_filter_level_count;
};

struct flatten_item
{
    int bf_instruct_link;
    int true_instruct;
    int false_instruct;
};


struct master_filter master_filter;
extern datalink_info link_info;

void initFilter();
int addFilter(const char * filter_string, int filter_id, int pipe, int control_pipe);
int removeFilter(int end_node_id);
void removeAllFilter();
int sendToAllNode(register const u_char *p,unsigned int buflen, struct timeval ts);

void flattenGraphTraversal(struct flatten_item * graph, struct bpf_program * filter, int indice, int level);
void traversal();
void innerTraversal(const char * pre,struct filter_item * i, int level);

int capture_setFileMode();
int capture_setSpeed(uint8 speed);
int capture_pause();
int capture_resume();
int capture_flush();
int capture_kill();

char * filter_item_image(const struct filter_item * p);

#endif
