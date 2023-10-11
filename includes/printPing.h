#ifndef PRINTPING_H
# define PRINTPING_H
# include "creationRequest.h"

void printFirstlinePing(const char* localhost, const char* ip, const int id);
void printRecvLine(int reception, struct msghdr msg, struct timeval start, struct timeval end);
void printRecapByHostname(t_recapPing recap);
double getDelay( struct timeval start, struct timeval end);


#endif