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


typedef struct  s_icmpPacket{
    struct icmphdr icmp_header;
    char data[DATA_SIZE];
}               t_icmpPacket;

int creationOfRequest(const char* host, bool isverbose);

#endif