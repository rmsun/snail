#include <iostream>
#include <sstream>
#include <fstream>

#include "Downloader.h"
#include "Socket.h"

Downloader::Downloader()
{
}

Downloader::~Downloader()
{
}

void Downloader::init( std::string aUrl )
{
	mUrl = aUrl;
	parseUrl();
}

void Downloader::parseUrl()
{
	std::cout << "url is : " << mUrl << std::endl;
	size_t pos = mUrl.find( '/'  );
	if( std::string::npos != pos )
	{
		std::cout << "the position of first '/' is: " << pos << std::endl;
		mHostName.assign( mUrl, 0, pos );
		std::cout << "host name is: " << mHostName << std::endl;
		mPath.assign( mUrl, pos, mUrl.size() - pos + 1 );
		std::cout << "path is: " << mPath << std::endl;
	}
	
	pos = mUrl.find_last_of( '/' );
	if( std::string::npos != pos )
	{
		std::cout << "the postion of last '/' is: " << pos << std::endl;
		mFileName.assign( mUrl, pos + 1, mUrl.size() - pos );
		std::cout << "file name is: " << mFileName << std::endl;
	}
}

void Downloader::start()
{
	Socket sock( mHostName );
	sock.connect();
	std::cout << "socket is: " << sock.getSocket() << std::endl;

	//char request[512];
	//snprintf( request, 512, "GET %%2Fimages%%2F9780321334879%%2Fsamplepages%%2F0321334876%%2Epdf HTTP/1.1\r\n\r\n" );
	std::string request;
	request.append( "GET " );
	//request.append( "%2fimages%2f9780321334879%2fsamplepages%2f0321334876%2epdf" );
	request.append( "/images/9780321334879/samplepages/0321334876.pdf" );
	request.append( " HTTP/1.1\r\n" );
	request.append( "Host: ptgmedia.pearsoncmg.com\r\n" );
	request.append( "Accept: */*\r\n" );
	request.append( "\r\n" );

	std::cout << "the request is: " << request << std::endl;
   
	char recvBuffer[1024];
	size_t totalSize = 0;
	size_t recvedSize = 0;
	size_t currentSize = 0;

	send( sock.getSocket(), request.c_str(), 512, 0 );

	int i = 0;
	bool recvedHttpHeader = false;
	std::ofstream file( mFileName.c_str(), std::ios::binary );
	if( !file.is_open() )
	{
		std::cout << "create file fail!!!" << std::endl;
		exit( 1 );
	}
	float percent = 0.0;
	do
	{
		currentSize = recv(  sock.getSocket(), recvBuffer + i, 1024, 0 );
		//recvedSize += currentSize;
		//std::cout << " current size is: " << currentSize << std::endl;
		if( currentSize > 0 )
		{
			//std::cout << "content is: " << recvBuffer << std::endl;
			if( false == recvedHttpHeader )
			{
				i = parseHttpResponse( recvBuffer, currentSize );
				if( -1 == i)
				{
					i = currentSize;
				}
				else
				{
					std::cout << recvBuffer << std::endl;
					totalSize = getFileSize( recvBuffer );
					std::cout << "total size is: " << totalSize << std::endl;
					recvedHttpHeader = true;
					file.write( recvBuffer + i, currentSize - i + 1 );
					i = 0;
					recvedSize += currentSize - i + 1;
					//std::cout << "receiveed " << recvedSize << " bytes" << std::endl;
					percent = (float)recvedSize / (float)totalSize;
					printf( "%.1lf%%\r", percent * 100 );
				}
			}
			else
			{
				file.write( recvBuffer, currentSize );
				recvedSize += currentSize;
				//std::cout << "receiveed " << recvedSize << " bytes" << std::endl;
				i = 0;
				percent = (float)recvedSize / (float)totalSize;
				printf( "%.1lf%%\r", percent * 100 );
				if( recvedSize >= totalSize )
				{
					std::cout << "\ndownload complete..." << std::endl;
					break;
				}
			}
		}
		else if( currentSize == 0  )
		{
			std::cout << "server disconnect" << std::endl;
			break;
		}
		else
		{
			std::cout << "recv error !!!" << std::endl;
			exit( 1 );
		}
	}while( 1 );
	
	close( sock.getSocket() );
}

int Downloader::parseHttpResponse( char* aData, int aLength )
{
	char *c = aData;
	for( int i = 0; i < aLength; ++i )
	{
		if( *( c + i) == '\r' 
			&& *( c + i + 1 ) == '\n'
			&& *( c + i + 2 ) == '\r'
			&& *( c + i + 3 ) == '\n' )
		{
			return i + 4;
		}
	}
	return -1;
}

int Downloader::getFileSize( char* aData )
{
	char *c = aData;
	int i = 0;
	if( NULL != aData )
	{
		//for( ; *( c + i ) != '\r' && *( c + i + 1 ) != '\n' && *( c + i + 2 ) != '\r' && *( c + i + 3 ) != '\n'; ++i)
		for( ; ; ++i )
		{
			if( *( c + i ) == 'C'
				&& *( c + i + 1 ) == 'o'
				&& *( c + i + 2 ) == 'n'
				&& *( c + i + 3 ) == 't'
				&& *( c + i + 4 ) == 'e'
				&& *( c + i + 5 ) == 'n'
				&& *( c + i + 6 ) == 't'
				&& *( c + i + 7 ) == '-'
				&& *( c + i + 8 ) == 'L'
				&& *( c + i + 9 ) == 'e'
				&& *( c + i + 10 ) == 'n'
				&& *( c + i + 11 ) == 'g'
				&& *( c + i + 12 ) == 't'
				&& *( c + i + 13 ) == 'h'
				&& *( c + i + 14 ) == ':'
				&& *( c + i + 15 ) == ' ' )
			{
				break;
			}
		}
		std::cout << "i = " << i << std::endl;
		char temp[64];
		int j = 0;
		for( ; *( c + i + 16 +j ) != '\r'; ++j )
		{
			temp[j] = *( c + i + 16 + j );
		}
		temp[j] = '\0';
		int contentLength = atoi( temp );
		return contentLength;
	}
	return 0;
}
