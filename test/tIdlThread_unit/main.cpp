#include <iostream>
using namespace std;
#include <windows.h>

#include "thread.h"
using namespace Vdl;

#include "method.h"

class ThreadFotUnit : public Thread
{
public:
	ThreadFotUnit()
		: m_increaseQueueCounter(0), 
		Thread("ThreadFotUnit")
	{
		
	}
	~ThreadFotUnit()
	{

	}

public:
	bool Init()
	{
		return Thread::Start();
	}

	void UnInit()
	{
		Thread::Stop();
	}

	bool Jump(int eventCounter = 911)
	{
		class JumpMethodRequest : public MethodRequest
		{
		public:
			JumpMethodRequest(ThreadFotUnit& thread, int eventCounter)
				: m_thread(thread), m_eventCounter(eventCounter)
			{

			}

		public:
			virtual void call()
			{
				m_thread.OnJump(m_eventCounter);
			}

		private:
			ThreadFotUnit& m_thread;
			const int m_eventCounter;
		};

		return Thread::PushMethod(new JumpMethodRequest(*this, eventCounter));
	}

private:
	void OnJump(int eventCounter)
	{
		cout << "ThreadFotUnit::OnJump eventCounter: " << eventCounter << endl;
	}

public:
	virtual void TimeOut()
	{
		cout << "ThreadFotUnit::TimeOut m_increaseQueueCounter: " << ++m_increaseQueueCounter << endl;
	}

private:
	int m_increaseQueueCounter;
};

int main(int argc, char** argv)
{
	ThreadFotUnit test;

	// test will create two thread:
	// 1: io-service thread: it work to deal with io dispatch. ps: event and timeout dispatch;
	// 2: work-service thread: it work to deal with basic service entity;
	if(!test.Init())
	{
		test.UnInit();
		return 1;
	}

	// right now, there have three thread. two for test; one for main.  
	for(int i = 0;i < 10;++i)
	{
		test.Jump();
		Sleep(3000);
	}

	system("pause");

	test.UnInit();
	return 0;
}
