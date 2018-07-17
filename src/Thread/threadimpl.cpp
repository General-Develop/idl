#include <boost/thread/thread.hpp>
#include <boost/bind/bind.hpp>
#include <iostream>

#include "threadimpl.h"
using namespace Vdl;

#include "thread.h"
#include "method.h"

Vdl::ThreadImpl::ThreadImpl( Thread& thread, const std::string& name )
	: m_thread(thread), m_taskName(name), m_interval(1), m_methodCount(0), 
	m_strand(m_io.get_executor()), m_pTimer(NULL), 
	m_isActived(false), m_isStarted(false), m_isStarteSuccess(false)
{

}

Vdl::ThreadImpl::~ThreadImpl()
{

}

bool Vdl::ThreadImpl::Start( boost::uint32_t second )
{
	if (!m_thread.InitializeTask())
	{
		m_thread.ExitTask();
		std::cerr << "Thread InitializeTask() failed, named : " << m_taskName << std::endl;
		return false;
	}
	m_interval = second;
	m_pTimer = new boost::asio::deadline_timer(m_io, boost::posix_time::seconds(m_interval));
	m_pThread = new boost::thread(boost::bind(&ThreadImpl::S_svc, this));
	m_isActived = true;

	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		while (!m_isStarted)
		{
			boost::posix_time::seconds const delay(5);
			boost::system_time const start=boost::get_system_time();
			boost::system_time const timeout=start+delay;
			m_conditon.timed_wait(m_mutex, timeout);
		}
	}

	return m_isStarteSuccess;
}

void Vdl::ThreadImpl::Stop()
{
	m_isActived = false;
	m_io.stop();
	if (NULL != m_pThread)
	{
		m_pThread->join();
		delete m_pThread;
		m_pThread = NULL;
	}
	delete m_pTimer;
	m_pTimer = NULL;
	m_thread.ExitTask();
	m_isStarteSuccess = false;
	m_isStarted = false;
}

void Vdl::ThreadImpl::TimeOut()
{

}

bool Vdl::ThreadImpl::PushMethod( MethodRequestPtr methodPtr )
{
	if (0 == methodPtr.get() || !m_isActived)
	{
		return false;
	}
	m_io.post(boost::bind(&ThreadImpl::S_HandleJob, this, methodPtr));
	++m_methodCount;
	const long count = m_methodCount.operator long();
	if (2048 < count)
	{
		std::cout << "Over 2048 method waited in the thread named : " 
			<< m_taskName << " Method type : " << GetMethodName(*methodPtr) 
			<< " Method count : " << count << std::endl;
	}
	return true;
}

const std::string& Vdl::ThreadImpl::GetName() const
{
	return m_taskName;
}

bool Vdl::ThreadImpl::IsRunning() const
{
	return m_isActived;
}

void Vdl::ThreadImpl::Svc()
{
	m_isStarteSuccess = m_thread.InitializeThread();
	{
		boost::lock_guard<boost::mutex> guard(m_mutex);
		m_isStarted = true;
		m_conditon.notify_all();
	}

	if (!m_isStarteSuccess)
	{
		m_thread.ExitThread();	
		std::cerr << "Thread InitializeThread() failed, named :  " << m_taskName << std::endl;
		return;
	}

	boost::asio::io_service::work permanence(m_io);
	if (0 != m_interval)
	{
		m_pTimer->async_wait(boost::bind(&ThreadImpl::S_TimeOut, this));
	}
	m_io.run();
	m_thread.ExitThread();	
}

void Vdl::ThreadImpl::S_svc( ThreadImpl* pImpl )
{
	pImpl->m_thread.Svc();
}

void Vdl::ThreadImpl::S_TimeOutJob( ThreadImpl* pImpl )
{
	pImpl->m_strand.post(boost::bind(&ThreadImpl::S_TimeOut, pImpl), std::allocator<void>());
}

void Vdl::ThreadImpl::S_HandleJob( ThreadImpl* pImpl,MethodRequestPtr methodPtr )
{
	methodPtr->call();
	--(pImpl->m_methodCount);
}

void Vdl::ThreadImpl::S_TimeOut( ThreadImpl* pImpl )
{
	pImpl->m_thread.TimeOut();
	pImpl->m_pTimer->expires_from_now(boost::posix_time::seconds(pImpl->m_interval));
	pImpl->m_pTimer->async_wait(boost::bind(&ThreadImpl::S_TimeOutJob, pImpl));
}

std::string Vdl::ThreadImpl::GetMethodName( const MethodRequest& method )
{
	std::string name;

	try
	{
		name = typeid(method).name();
	}
	catch (...)
	{

	}

	return name;
}
