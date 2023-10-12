#ifndef CREATIONREQUEST_H
# define CREATIONREQUEST_H
# define PACKET_SIZE 64
# define ICMP_HEADER_SIZE 8
# define DATA_SIZE (PACKET_SIZE - ICMP_HEADER_SIZE)

# include <stdbool.h>
# include <netdb.h>
# include <errno.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <arpa/inet.h>
# include <netinet/ip_icmp.h>
# include <sys/time.h>
# include <signal.h>
# include <math.h>
# include "parser.h"

struct icmpheadr {
    unsigned char type;
    unsigned char code;
    unsigned short checksum;
    unsigned short id;
    unsigned short seq;
};


typedef struct  s_icmpPacket{
    struct icmpheadr icmp_header;
    char data[DATA_SIZE];
}               t_icmpPacket;

typedef struct	s_recapPing{
	char		hostname[1024];
	uint32_t	totalPacket;
	uint32_t	totalReceive;
	double		min;
	double		max;
	double		mean;
	double		stddev;
}				t_recapPing;

int creationOfRequest(const char* host, parsedData_t setup);

#endif