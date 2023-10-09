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


void printFirstlinePing(const char* localhost, const char* ip, const int id)
{
    printf("PING %s (%s): 56 data bytes", localhost, ip);
    if (id != -1)
    {
        printf(", id 0x%.4x=%.4d\n", id, id);
    }
    else
    printf("\n");
}

int creationOfDestinationByHostname(const char* host, struct addrinfo **result, const int uid)
{
    struct              addrinfo hints;
    int                 status;
    char                ip[INET_ADDRSTRLEN];
    char                realHost[NI_MAXHOST];
    struct sockaddr_in* ipv4;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    status = getaddrinfo(host, NULL, &hints, result);
    if (status != 0)
        return (status);
    ipv4 = (struct sockaddr_in *)(*result)->ai_addr;
    inet_ntop(AF_INET, (void*)&(ipv4->sin_addr), ip, sizeof(ip));
    status = getnameinfo((*result)->ai_addr, sizeof(struct sockaddr_in), realHost, NI_MAXHOST, NULL, 0, 0);
    if (status != 0)
        return (status);
    printFirstlinePing(realHost, ip, uid);
    return (0);
}

void    setPacketIMCP(t_icmpPacket *packet)
{
    static int          sequence = 0;

    ft_memset(packet, 0, sizeof(t_icmpPacket));
    packet->icmp_header.type = ICMP_ECHO;
    packet->icmp_header.code = 0;
    packet->icmp_header.checksum = 0;
    packet->icmp_header.un.echo.id = getpid();
    packet->icmp_header.un.echo.sequence = sequence++;
    ft_strlcpy(packet->data, "Hello, ICMP Echo!", sizeof("Hello, ICMP Echo!"));
    packet->icmp_header.checksum = checksum(&packet, sizeof(packet));
}

int recvEchoRply(int sockfd, u_int16_t seq)
{
    struct  msghdr msg;
    struct  iovec iov;
    char    buffer[64];
    
    (void)seq;
    ft_memset(&msg, 0, sizeof(msg));
    ft_memset(buffer, 0, sizeof(buffer));
    iov.iov_base = buffer;
    iov.iov_len = sizeof(buffer);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    
    printf("je rentre dedans\n");
    ssize_t bytes_received = recvmsg(sockfd, &msg, 0);
    if (bytes_received < 0)
    {
        printf("timeout => %li\n", bytes_received);
        printf("%d\n", errno);
        perror("recvmsg");
        return (1);
    }
    return (0);
}

int sendingRequest(int sockfd, struct sockaddr_in  target)
{
     t_icmpPacket packet;
    int res;
    
    setPacketIMCP(&packet);
    if ((res = sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&target, sizeof(target))) == -1)
    {
        perror("sendto");
    }
    printf("res = %d\n", res);
    recvEchoRply(sockfd, packet.icmp_header.un.echo.sequence);
    return (0);
}

int creationOfRequest(const char* host, bool isverbose) {
    struct sockaddr_in  target;
    struct addrinfo     *result;
    struct timeval      timeout;
    int                 sockfd;
    int                 status;
    
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
    {
        perror("socket");
        return (1);
    }
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
        perror("setsockopt");
        close(sockfd);
        return (1);
    }

    status = creationOfDestinationByHostname(host, &result, (isverbose) ? getpid() : -1);
    if (status)
    {
        if (status == -2)
            fprintf(stderr, "ft_ping: unknown host\n");
        else
            fprintf(stderr, "Error: %s => %d\n", gai_strerror(status), status);
        return (1);
    }
    ft_memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr = ((struct sockaddr_in *)((result)->ai_addr))->sin_addr;

    sendingRequest(sockfd, target);
    close(sockfd);
    freeaddrinfo(result);
    return (0);
}
