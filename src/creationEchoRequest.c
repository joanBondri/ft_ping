#include "creationRequest.h"
#include "libft.h"

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
        sum += *buf++;

    if (len == 1)
        sum += *(unsigned char *)buf;

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

int creationOfDestinationByHostname(const char* host, struct addrinfo **result)
{
    struct  addrinfo hints;
    int     status;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(host, NULL, &hints, result);
    if (status != 0)
        return (status);
    return (0);
}

void    setPacketIMCP(t_icmpPacket *packet)
{
    static int          seq = 0;

    ft_memset(&packet, 0, sizeof(packet));
    packet->icmp_header.type = ICMP_ECHO;
    packet->icmp_header.code = 0;
    packet->icmp_header.checksum = 0;
    packet->icmp_header.un.echo.id = getpid();
    packet->icmp_header.un.echo.sequence = seq++;
    ft_strlcpy(packet->data, "Hello, ICMP Echo!", sizeof("Hello, ICMP Echo!"));
    packet->icmp_header.checksum = checksum(&packet, sizeof(packet));  
}

int creationOfRequest(const char* host) {
    struct sockaddr_in  target;
    struct addrinfo     *result;
    t_icmpPacket        packet;
    int                 sockfd;
    int                 status;
    
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("socket");
        return (1);
    }
    status = creationOfDestinationByHostname(host, &result);
    if (status)
    {
        if (status == -2)
            fprintf(stderr, "ft_ping: unknown host\n");
        else
            fprintf(stderr, "Error: %s => %d\n", gai_strerror(status), status);
        return (1);
    }
    setPacketIMCP(&packet);
    ft_memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr = ((struct sockaddr_in *)((result)->ai_addr))->sin_addr;
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&target, sizeof(target)) == -1)
        perror("sendto");
    printf("ICMP Echo Request sent to %s\n", host);
    close(sockfd);
    freeaddrinfo(result);
    return (0);
}
