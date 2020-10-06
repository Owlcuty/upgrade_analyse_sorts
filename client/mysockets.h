#include <iostream>
#include <unistd.h>

#include <netinet/in.h>
#include <sys/socket.h>

#include "arpa/inet.h"

class Socket
{
public:
	explicit Socket();
	int get_sock_fd();
	
	~Socket();
private:
	int m_sock = -1;
};