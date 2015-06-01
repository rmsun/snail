#ifndef DOWNLOADER_H_
#define DOWNLOADER_H_

#include <string>

class Downloader
{
public:
	Downloader();
	~Downloader();

public:
	void init( std::string aUrl );
	void start();
	void parseUrl();
	int parseHttpResponse( char* aData, int aLength );
	int getFileSize( char* aData );
	std::string mFileName;
	std::string mHostName;
	std::string mUrl;
	std::string mPath;
};

#endif
