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
    TODO SEGMENTLIB : 
        -gerer la taille max du buffer
        -gerer le tour du compteur de la sequence
            c'est embetant pour des comparaisons de sequence
        -gerer le réordonnement des syn/ack, pour l'instant on ne gère que les datas
            -les syn c'est indispensable
            -utilité des ack et autre?
        -gérer la connexion close simultanément
            lorsqu'on a reçu au moins un flag fin, plus besoin de contrôler les segments avec des numéros de séquence supérieur à celui du fin
                il n'y aura tout simplement plus de données après.
        
        -lorsqu'une connexion est condamnée, attendre le délais tcp max avant de la retirée
            -5 minutes
        
*/

#ifndef _SEGMENTLIB_H
#define _SEGMENTLIB_H

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
#include "../capturefilterlib/capturefilterlib.h"
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#define MAXBUFFSIZE 2048 /*TODO SEGMENTLIB not used*/
#define SEGMAXBUFFBYSOCK 100 /*[1, 255]*/
#define SEGTIMEOUT 80

#define SEG_FLAGS_END 0x01
#define SEG_FLAGS_BROKEN 0x02

extern datalink_info link_info;

typedef struct
{
    uint32 seq;
    uint8 * data;
    int dsize;
    void * next_buffer;
    void * previous_buffer;
} next_buffer;

typedef struct sequence_entry
{
    uint16 port_src;
    uint16 port_dst;
    
    next_buffer * linked_buffer;
    uint32 seq; /*waited sequence*/
    struct timeval mod_time;
    uint8 flags;
    uint8 count;
    
}sequence_entry;

typedef struct sequence_entry_ipv6
{
    uint8 ip_src[16];
    uint8 ip_dest[16];
    
    struct sequence_entry seq;
    
    struct sequence_entry_ipv6 * next;
    struct sequence_entry_ipv6 * previous;
}sequence_entry_ipv6;

typedef struct sequence_entry_ipv4
{
    uint32 ip_src;
    uint32 ip_dest;
    
    struct sequence_entry seq;
    
    struct sequence_entry_ipv4 * next;
    struct sequence_entry_ipv4 * previous;

} sequence_entry_ipv4;

sequence_entry_ipv4 * seq_start_ipv4;
sequence_entry_ipv4 * seq_last_ipv4;

sequence_entry_ipv6 * seq_start_ipv6;
sequence_entry_ipv6 * seq_last_ipv6;

#ifdef DEBUG_MEMORY_SEG
extern int count_entry, cont_buffer;
#endif

sniff_ip  * last_header_ip;
sniff_ip6 * last_header_ip6;
sniff_tcp * last_header_tcp;

sequence_entry_ipv4 * getEntry(uint32 ip_src, uint32 ip_dest, uint16 port_src, uint16 port_dst);
sequence_entry * createEntry(uint32 ip_src, uint32 ip_dest, uint16 port_src, uint16 port_dst, uint32 se);

sequence_entry_ipv6 * getEntry_ipv6(uint8 * ip_src, uint8 * ip_dest, uint16 port_src, uint16 port_dst);
sequence_entry * createEntry_ipv6(uint8 * ip_src, uint8 * ip_dest, uint16 port_src, uint16 port_dst, uint32 se);

int addData(sequence_entry * entry, uint32 sequence_number,const uint8 * datas, int data_length);
next_buffer * createNextBuffer(uint32 sequence_number,const uint8 * datas, int data_length, next_buffer * next, next_buffer * previous);
uint8 * forgeSegment_ipv4(sequence_entry_ipv4 * entry, next_buffer * buffer, int datalink_size);
uint8 * forgeSegment_ipv6(sequence_entry_ipv6 * entry, next_buffer * buffer, int datalink_size);
void flushAllSegment();
void removeEntry_ipv4(sequence_entry_ipv4 * entry);
void removeEntry_ipv6(sequence_entry_ipv6 * entry);
void cleanData(sequence_entry * entry);

int addNewSegment_ipv4(sniff_tcp * header_tcp, sniff_ip * header_ip, const uint8 * datas);
int addNewSegment_ipv6(sniff_tcp * header_tcp, sniff_ip6 * header_ip6, const uint8 * datas);

void sendReadySegment_ipv4(sniff_tcp * tcp, sniff_ip * ip, struct timeval t);
void sendReadySegment_ipv6(sniff_tcp * tcp, sniff_ip6 * ip6, struct timeval t);

void setIPv4TCP(sniff_ip * ip, sniff_tcp * tcp);
void setIPv6TCP(sniff_ip6 * ip, sniff_tcp * tcp);

void forwardSegmentInBuffer(struct timeval t);

#endif