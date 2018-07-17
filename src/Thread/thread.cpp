#include "thread.h"
using namespace Vdl;

#include "threadimpl.h"
#include "method.h"

Vdl::Thread::Thread( const std::string& name )
{
	m_pThreadImpl = new ThreadImpl(*this, name);
}

Vdl::Thread::~Thread()
{
	delete m_pThreadImpl;
}

bool Vdl::Thread::Start( boost::uint32_t second /*= 1*/ )
{
	return m_pThreadImpl->Start(second);
}

void Vdl::Thread::Stop()
{
	m_pThreadImpl->Stop();
}

void Vdl::Thread::TimeOut()
{
	m_pThreadImpl->TimeOut();
}

bool Vdl::Thread::PushMethod( MethodRequestPtr methodPtr )
{
	return m_pThreadImpl->PushMethod(methodPtr);
}

const std::string& Vdl::Thread::GetName() const
{
	return m_pThreadImpl->GetName();
}

bool Vdl::Thread::IsRunning() const
{
	return m_pThreadImpl->IsRunning();
}

bool Vdl::Thread::InitializeTask()
{
	return true;
}

void Vdl::Thread::ExitTask()
{
	
}

bool Vdl::Thread::InitializeThread()
{
	return true;
}

void Vdl::Thread::ExitThread()
{

}

void Vdl::Thread::Svc()
{
	m_pThreadImpl->Svc();
}
