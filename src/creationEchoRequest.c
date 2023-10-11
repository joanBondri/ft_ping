#include "creationRequest.h"
#include "libft.h"
#include "printPing.h"

bool keeprunning = true;

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

int creationOfDestinationByHostname(const char* host, struct addrinfo **result, const int uid, t_recapPing* recap)
{
    struct addrinfo		hints;
    int                 status;
    char                ip[INET_ADDRSTRLEN];
    char                realHost[NI_MAXHOST];
    struct sockaddr_in* ipv4;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
	hints.ai_protocol = IPPROTO_ICMP;
    status = getaddrinfo(host, NULL, &hints, result);
    if (status != 0)
        return (status);
    ipv4 = (struct sockaddr_in *)(*result)->ai_addr;
    inet_ntop(AF_INET, (void*)&(ipv4->sin_addr), ip, sizeof(ip));
    status = getnameinfo((*result)->ai_addr, sizeof(struct sockaddr_in), realHost, NI_MAXHOST, NULL, 0, 0);
    if (status != 0)
        return (status);
    printFirstlinePing(realHost, ip, uid);
	ft_strlcpy(recap->hostname, realHost, sizeof(realHost));
    return (0);
}

void    setPacketIMCP(void* buff, int req)
{
	struct icmphdr		*packet;

	packet = (struct icmphdr*)buff;
    ft_memset(packet, 0, sizeof(struct icmphdr));
    packet->type = ICMP_ECHO;
    packet->code = 0;
	packet->checksum = 0;
	packet->id = htons(getpid());
	packet->seq = htons(req);
	gettimeofday((struct timeval *)(buff + 8), NULL);
	packet->checksum = checksum(buff, PACKET_SIZE);
}


void setupReception (struct  msghdr* msg,struct iovec* iov, char* buffer, size_t bufferlen)
{
    ft_memset(msg, 0, sizeof(*msg));
    ft_memset(buffer, 0, bufferlen);
	msg->msg_iov = iov;
    msg->msg_iov->iov_base = buffer;
    msg->msg_iov->iov_len = bufferlen;
    msg->msg_iovlen = 1;
}


void handleSignal(int yop)
{
	(void)yop;
	keeprunning = false;
}

int sendingRequest(int sockfd, struct sockaddr_in  target, t_recapPing* recap, int req)
{
    char packet[PACKET_SIZE];
	struct timeval start;
	struct timeval end;
    struct  msghdr msg;
    struct  iovec iov;
    char    buffer[1024];

    
	setupReception(&msg, &iov, buffer, sizeof(buffer));
    setPacketIMCP(&packet, req);
	gettimeofday(&start, NULL);
    if (sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&target, sizeof(target)) == -1)
        perror("sendto");
	recap->totalPacket++;
    ssize_t bytes_received = recvmsg(sockfd, &msg, 0);
	gettimeofday(&end, NULL);
    if (bytes_received < 0)
    {
        perror("recvmsg");
        return (1);
    }
	printRecvLine(bytes_received, msg, start, end);
	double delay = getDelay(start, end);
	double sum = recap->stddev * recap->stddev * recap->totalReceive;
	recap->totalReceive++;
	recap->max = recap->max > delay ? recap->max : delay;
	recap->min =( recap->min < delay && recap->min != 0) ? recap->min : delay;
	recap->mean = (recap->mean * (recap->totalReceive - 1) + delay) / recap->totalReceive;
	recap->stddev = sqrt((sum + (delay - recap->mean) * (delay - recap->mean)) / recap->totalReceive);
	sleep(1);
    return (0);
}

int creationOfRequest(const char* host, parsedData_t setup) {
    struct sockaddr_in  target;
    struct addrinfo     *result;
    struct timeval      timeout;
    int                 sockfd;
    int                 status;
	t_recapPing			recap = {0};
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

    status = creationOfDestinationByHostname(host, &result, (setup.verbose) ? getpid() : -1, &recap);
    if (status)
    {
		fprintf(stderr, "ft_ping: unknown host\n");
        return (1);
    }
    ft_memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr = ((struct sockaddr_in *)((result)->ai_addr))->sin_addr;
	signal(SIGINT, handleSignal);
	for (int seq = 0; keeprunning ; seq++)
	{
		sendingRequest(sockfd, target, &recap, seq);
	}
	printRecapByHostname(recap);
    close(sockfd);
    freeaddrinfo(result);
    return (0);
}
