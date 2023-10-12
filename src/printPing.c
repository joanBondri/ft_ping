
#include "printPing.h"
#include "libft.h"

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

double getDelay( struct timeval start, struct timeval end)
{
	return (double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_usec - start.tv_usec) / 1000.0;
}

void printRecvLine(int reception, struct msghdr msg, struct timeval start, struct timeval end)
{
	struct ip*		ip_header;
	struct icmpheadr*	icmp_header;
	int 			ttl;
	char 			source_ip[INET_ADDRSTRLEN];
	int 			sequence;
	double 			delay;

	ip_header = (struct ip *)msg.msg_iov[0].iov_base;
	icmp_header = (struct icmpheadr *)(msg.msg_iov[0].iov_base + (ip_header->ip_hl << 2));
	ttl = ip_header->ip_ttl;
	inet_ntop(AF_INET, &(ip_header->ip_src), source_ip, INET_ADDRSTRLEN);
	sequence = ntohs(icmp_header->seq);
	delay = getDelay(start, end); 
	printf("%d bytes from %s: icmp_seq=%d ttl=%u time=%.2f ms\n", reception, source_ip, sequence, ttl, delay);
}

void printRecapByHostname(t_recapPing recap)
{
	int	percent;

	percent =(recap.totalPacket - recap.totalReceive)/ recap.totalPacket * 100;
	printf("--- %s ft_ping statistics ---\n", recap.hostname);
	printf("%d packets transmitted, %d packets received, %d%% packet loss\n", recap.totalPacket, recap.totalReceive, percent);
	if (recap.totalReceive == 0)
		return ;
	printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", recap.min, recap.mean, recap.max, recap.stddev);
}

void    printFlood(int number)
{
	if (number < 1)
		return ;
	printf("\r");
	fflush(stdout);
	while(number--)
		ft_putchar_fd('.', 1);
}