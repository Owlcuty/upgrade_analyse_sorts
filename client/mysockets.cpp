#include "mysockets.h"


Socket::Socket() :
m_sock(socket(AF_INET, SOCK_STREAM, 0))
{
	if (m_sock == -1)
	{
		throw std::runtime_error("failed to create socket");
	}
}

int Socket::get_sock_fd()
{
	return m_sock;
}

Socket::~Socket()
{
	close(m_sock);
}