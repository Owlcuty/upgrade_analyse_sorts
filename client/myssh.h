#include <iostream>
#include "libssh2.h"
#include "mysockets.h"


class LibSSH2
{
public:
	static LibSSH2& Instance();
private:
	explicit LibSSH2();
	~LibSSH2();
	
	LibSSH2(LibSSH2 const&);
	LibSSH2& operator= (LibSSH2 const&);
};

class Session
{
public:
	Session(const bool enable_compression);
	~Session();
private:
	LIBSSH2_SESSION *m_session;
};

class SimpleChannel
{
public:
	SimpleChannel(int socket_fd, LIBSSH2_SESSION* session);
	void SendEof();
	~SimpleChannel();
private:
	void CloseChannel();
	LIBSSH2_CHANNEL *m_channel;
};