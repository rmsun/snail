#include "Thread.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <iostream>

void add( int i, int j )
{
	std::cout << i << " + " << j << " = " << i + j << std::endl;
}

class Calc
{
public:
	Calc(){};
	~Calc(){};
public:
	void add( int i, int j )
	{
		std::cout << i << " + " << j << " = " << i + j << std::endl;
	};
};

int main( int argc, char** argv )
{
	Calc testCalc;
	Thread* thread1 = new Thread();
	Thread* thread2 = new Thread();
	//thread1->init( boost::bind( &add, 100, 100 ) );
	//thread2->init( boost::bind( &add, 200, 200 ) );
	thread1->init( boost::bind( &Calc::add, &testCalc, 100, 200 ) );
	thread2->init( boost::bind( &Calc::add, &testCalc, 200, 200 ) );
	thread1->run();
	thread2->run();
	thread1->join();
	thread2->join();
			
	return 0;
}
