/**
*	fbThread.cpp
*	fbThread Source
*	@author Byhron Heads
*	@date March 5, 2008
*/

#include "fbThread.h"

/**
*	fbThread
*	Default constructor
*	@note 3/5/08 Initilize vars to false
*/
fbThread::fbThread():_running(false), _stopping(false), _paused(false), _hThread(NULL)
{
}

/**
*	~fbThread
*	Destructor
*/
fbThread::~fbThread()
{
	if(_running)
		forceStop();
}

/**
*	start
*	starts the thread
*/
void fbThread::start()
{
	if(_running)
		return;
#ifdef Win32
	_hThread = CreateThread(NULL, 0, threadStart, this, 0, NULL);
	//if(_hThread == NULL)
	//	ERROR
#else
	int ret = pthread_create(&_hThread, NULL, threadStart, this);
	//if(ret != 0)
	//	ERROR
#endif
}

/**
*	stop
*	tell thread to stop
*/
void fbThread::stop()
{
	if (!_running)
		return;
	_stopping = true;
}

/**
*	forceStop
*	kills the thread, use with caution, may leave resources open
*/
void fbThread::forceStop()
{
	if (!_running)
		return;
#ifdef Win32
	DWORD ret = TerminateThread(_hThread, 0);
	//if(ret == -1)
	//	ERROR
#else
	int ret = pthread_cancel(_hThread);
	//if(ret != 0)
	//	ERROR	
#endif
	_running = false;
	_stopping = false;
	_paused = false;
	_hThread = NULL;
}

/**
*	pause
*	pause the thread
*/
void fbThread::pause()
{
	if (!_running || _paused)
		return;
	
#ifdef Win32
	DWORD ret = SuspendThread(_hThread);
	//if(ret == -1)
	//	error
#else
#endif
	_paused = true;
}

/**
*	resume
*	resume a _paused thread
*/
void fbThread::resume()
{
	if (!_running || !_paused)
		return;
#ifdef Win32
	DWORD ret = ResumeThread(_hThread);
	//if(ret == -1)
	//	ERROR
#else
#endif
	_paused = false;
}
	
/**
*	isRunning
*	is the thread running
*	@return true if thread is running
*/
bool fbThread::isRunning()
{
	return _running;
}

/**
*	ispaused
*	is the thread paused
*	@return true if thread is paused
*/
bool fbThread::isPaused()
{
	return _paused;
}

/**
*	isStopping
*	has the thread be asked to stop
*	@return true if thread is asked to stop
*/
bool fbThread::isStopping()
{
	return _stopping;
}


#ifdef Win32
/**
*	threadStart
*	real thread function
*	@param thread thread to run
*/
DWORD WINAPI fbThread::threadStart(LPVOID thread)
{
	fbThread* t = (fbThread*)thread;
	t->_running = true;
	t->run();	/// < cast thread and call run
	t->_running = false;
	t->_stopping = false;
	t->_paused = false;
	t->_hThread = NULL;
	return 0;
}
#else
/**
*	threadStart
*	real thread function
*	@param thread thread to run
*/
void* fbThread::threadStart(void* thread)
{
	fbThread* t = (fbThread*)thread;
	t->_running = true;
	t->run();	/// < cast thread and call run
	t->_running = false;
	t->_stopping = false;
	t->_paused = false;
	t->_hThread = NULL;
	return 0;
}
#endif

/**
*	sleep
*	Thread sleeps for given number of seconds
*	@param sec Number of seconds to sleep for
*/
void fbThread::sleep(int sec)
{
	if(!_running || _paused)
		return;
#ifdef Win32
	Sleep(sec * 1000);
#else
#endif
}

/**
*	usleep
*	Thread sleeps for given number of milliseconds
*	@param msec Number of milliseconds to sleep for
*/
void fbThread::usleep(int msec)
{
	if(!_running || _paused)
		return;
#ifdef Win32
	Sleep(msec);
#else
#endif
}

/**
*	yield
*	Thread yields allowing other threads to run
*/
void fbThread::yield()
{
#ifdef Win32
	Yield();
#else
	pthread_yield();
#endif
}

/**
*	run
*	virtual run function, overload for your own thread
*/
void fbThread::run()
{
}