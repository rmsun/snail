#include <iostream>
#include <stdlib.h>

#include "Downloader.h"
#include "Thread.h"

int main( int argc, char** argv )
{
	if ( NULL == argv[1] )
	{
		std::cout << "please input url!"  << std::endl;
		exit( 1 );
	}

	Downloader downLoader;
	std::string url( argv[1] );
	downLoader.init( url );
	int fileSize = 0;
	fileSize = downLoader.getFileSize();
	std::cout << "file's size is: " << fileSize << std::endl;
	if ( 0 == fileSize )
	{
		std::cout << "file's size is 0, error!!!" << std::endl;
		exit( 1 );
	}
	
	//int threadNumber = 2;
	Thread* thread1 = new Thread();
	Thread* thread2 = new Thread();
	thread1->init( boost::bind( &Downloader::start, &downLoader, 0, 0,  fileSize / 2) );
	thread2->init( boost::bind( &Downloader::start, &downLoader, 1, fileSize / 2 + 1, fileSize ) );
	thread1->run();
	thread2->run();
	thread1->join();
	thread2->join();
	//downLoader.start();

	return 0;
}
