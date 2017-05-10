#ifndef _RDT_FUNCS
#define _RDT_FUNCS

#include <sys/socket.h>


int rdt_socket(int addr_family, int type, int protocol);
int rdt_bind(int sockd, const struct sockaddr* localaddr, socklen_t addrlen);
int rdt_recv(int sockd, char* buffer, int bufflen, int flags, struct sockaddr* addrfrom, socklen_t* addrlen);
int rdt_send(int sockd, char* buffer, int bufflen, int flags, struct sockaddr* addrto, socklen_t addrlen);
int rdt_close(int fd);
int set_drop_q(int mils);
int set_corrupt_q(int mils);


#endif
