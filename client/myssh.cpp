#include "myssh.h"

LibSSH2& LibSSH2::Instance()
{
	static LibSSH2 lssh2;
	return lssh2;
}

LibSSH2::LibSSH2()
{
	if (libssh2_init(0) != 0)
	{
		throw std::runtime_error("libssh2 initialization failed");
	}
	else
	{
		printf("It's OK!");
	}
}

LibSSH2::~LibSSH2()
{
	printf("shutdown libssh2\n");
	libssh2_exit();
}

Session::Session(const bool enable_compression = 0) :
m_session(libssh2_session_init())
{
	if (m_session == nullptr)
	{
		throw std::runtime_error("failed to create libssh2 session");
	}
	
	libssh2_session_set_blocking(m_session, 0); 
	if (enable_compression)
	{
		libssh2_session_flag(m_session, LIBSSH2_FLAG_COMPRESS, 1);
	}
}

Session::~Session()
{
	libssh2_session_disconnect(m_session, "Shutdown libssh2 session");
	libssh2_session_free(m_session);
}

static int WaitSocket(int socket_fd, LIBSSH2_SESSION* session)
{
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	
	fd_set fd;
	
	FD_ZERO(&fd);
	FD_SET(socket_fd, &fd);
	
	int dir = libssh2_session_block_directions(session);
	
	fd_set *writefd = NULL;
	fd_set *readfd = NULL;
	
	if (dir & LIBSSH2_SESSION_BLOCK_INBOUND)
	{
		readfd = &fd;
	}
	if (dir & LIBSSH2_SESSION_BLOCK_OUTBOUND)
	{
		writefd = &fd;
	}
	
	int rc = select(socket_fd + 1, readfd, writefd, NULL, &timeout);
	
	return rc;
}


SimpleChannel::SimpleChannel(int socket_fd, LIBSSH2_SESSION* session)
{
	while ((m_channel = libssh2_channel_open_session(session)) == nullptr &&
			libssh2_session_last_errno(session) == LIBSSH2_ERROR_EAGAIN)
	{
		WaitSocket(socket_fd, session);
	}
	if (m_channel == nullptr)
	{
		throw std::runtime_error("Critical error while opening simple channel");
	}
}

void SimpleChannel::SendEof()
{
	if (libssh2_channel_send_eof(m_channel) == LIBSSH2_ERROR_EAGAIN)
	{
		fprintf(stderr, "%d:: Bad send eof\n", __LINE__);
	}
	if (libssh2_channel_wait_eof(m_channel) == LIBSSH2_ERROR_EAGAIN)
	{
		fprintf(stderr, "%d:: Bad wait (wtf) eof\n", __LINE__);
	}
}

SimpleChannel::~SimpleChannel()
{
	CloseChannel();
}

void SimpleChannel::CloseChannel()
{
	int rc = 0;
	libssh2_channel_close(m_channel);
	libssh2_channel_free(m_channel);
}