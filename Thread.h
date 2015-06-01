#ifndef THREAD_H_
#define THREAD_H_

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <pthread.h>
#include <stdlib.h>

class Thread
{
public:
	typedef boost::function<void()> threadFunc;
public:
	Thread();
	~Thread();
	
public:
	void init( threadFunc aFunc );
	void run();
	void join();

public:
	static void* start( void* arg );
	void callFunc();

private:
	threadFunc mThreadFunc;
	pthread_t mThreadId;
};

#endif
