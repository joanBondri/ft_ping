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
	printf("pass the getaddrinfo\n");
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

double getDelay( struct timeval start, struct timeval end)
{
	return (double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_usec - start.tv_usec) / 1000.0;
}

void printRecvLine(int reception, struct msghdr msg, struct timeval start, struct timeval end)
{
	struct ip*		ip_header;
	struct icmphdr*	icmp_header;
	int 			ttl;
	char 			source_ip[INET_ADDRSTRLEN];
	int 			sequence;
	double 			delay;

	ip_header = (struct ip *)msg.msg_iov[0].iov_base;
	icmp_header = (struct icmphdr *)(msg.msg_iov[0].iov_base + (ip_header->ip_hl << 2));
	ttl = ip_header->ip_ttl;
	inet_ntop(AF_INET, &(ip_header->ip_src), source_ip, INET_ADDRSTRLEN);
	sequence = ntohs(icmp_header->seq);
	delay = getDelay(start, end); 
	printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.2f ms\n", reception, source_ip, sequence, ttl, delay);
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

void printRecapByHostname(t_recapPing recap)
{
	float	percent;

	percent = recap.totalReceive / recap.totalPacket * 100.0;
	printf("--- %s ft_ping statistics ---\n", recap.hostname);
	printf("%d packets transmitted, %d packets received, %4f%% packet loss\n", recap.totalPacket, recap.totalReceive, percent);
	printf("round-trip min/avg/max/stddev = %.4f/%.4f/%.4f/%.4f ms\n", recap.min, recap.mean, recap.max, recap.stddev);
}

void handleSignal(int yop)
{
	printf("Received signal: %d\n", yop);
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
        if (status == -2)
            fprintf(stderr, "ft_ping: unknown host\n");
        else
            fprintf(stderr, "Error: %s => %d\n", gai_strerror(status), status);
        return (1);
    }
    ft_memset(&target, 0, sizeof(target));
    target.sin_family = AF_INET;
    target.sin_addr = ((struct sockaddr_in *)((result)->ai_addr))->sin_addr;
	void* res = signal(SIGINT, handleSignal);
	printf("signal res = %p\n", res);
	for (int seq = 0; true; seq++)
	{
		sendingRequest(sockfd, target, &recap, seq);
	}
	printRecapByHostname(recap);
    close(sockfd);
    freeaddrinfo(result);
    return (0);
}
