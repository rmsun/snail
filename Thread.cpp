#include "Thread.h"
#include <unistd.h>
#include <stdio.h>
#include <string.h>

Thread::Thread()
{

}

Thread::~Thread()
{

}

void* Thread::start( void* arg )
{
	((Thread*)arg)->callFunc();
}

void Thread::init( threadFunc aFunc )
{
	mThreadFunc = aFunc;
}

void Thread::run()
{
	int errorCode;
	errorCode = pthread_create( &mThreadId, NULL, Thread::start, this );
	if( 0!= errorCode )
	{
		fprintf( stderr, "create thread fail errorcode = %s\n", strerror( errorCode ) );
	}
}

void Thread::callFunc()
{
	mThreadFunc();
}

void Thread::join()
{
	pthread_join( mThreadId, NULL );
}
