#include <iostream>
#include <stdlib.h>

#include "Downloader.h"

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
	std::cout << "file's size is: " << downLoader.getFileSize() << std::endl;
	downLoader.start();

	return 0;
}
