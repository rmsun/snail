#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <string.h>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

class Socket
{
public:
	Socket( std::string aHostName  );
	~Socket();

public:
	std::string getHostIp();
	std::string getHostName();
	void connect();
	int getSocket();

private:
	std::string mHostIp;
	std::string mHostName;
	int mSocket;
};

#endif
