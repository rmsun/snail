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
	void start(int aBlock, int aStart, int aEnd);  // download a range of file
	void parseUrl();
	int parseHttpResponse( char* aData, int aLength );
	int getFileSize( char* aData );
	int getFileSize();
	std::string mFileName;
	std::string mHostName;
	std::string mUrl;
	std::string mPath;
    std::string mHost;
};

#endif
