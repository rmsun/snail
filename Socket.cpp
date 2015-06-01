#include "Socket.h"

Socket::Socket( std::string aHostName )
	: mHostName( aHostName )
{
	
}

Socket::~Socket()
{

}

void Socket::connect()
{
	struct addrinfo hints;
	struct addrinfo *result, *rp;
	int errorCode;
	std::string  httpDefaultPort( "80" );

	memset( &hints, 0, sizeof( struct addrinfo ) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = 0;
	hints.ai_protocol = 0;


	errorCode = getaddrinfo( mHostName.c_str(), httpDefaultPort.c_str(), &hints, &result );
	if( 0 != errorCode )
	{
		fprintf( stderr, "getaddrinfo errorcode = %s\n", gai_strerror(errorCode) );
	}

	for( rp = result; rp != NULL; rp = rp->ai_next )
	{
		mSocket = socket( rp->ai_family, rp->ai_socktype, rp->ai_protocol );
		if( -1 == mSocket)
		{
			continue;
		}
		if( -1 != ::connect( mSocket, rp->ai_addr, rp->ai_addrlen ) )
		{
			sockaddr_in sockin;
			memcpy( &sockin, &rp->ai_addr, sizeof( sockin ) );
			mHostIp = inet_ntoa( sockin.sin_addr );
			break;
		}
		close( mSocket );
	}

	if( NULL == rp )
	{
		fprintf( stderr, "could not connect\n" );
		exit( EXIT_FAILURE );
	}

	freeaddrinfo( result );
}

int Socket::getSocket()
{
	return mSocket;
}

std::string Socket::getHostName()
{
	return mHostName;
}

std::string Socket::getHostIp()
{
	return mHostIp;
}
