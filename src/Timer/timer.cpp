#include <boost/timer.hpp>
using namespace boost;

#include "timer.h"
using namespace Vdl;

class Timer::TimerImpl
{
public:
	TimerImpl() {}
	~TimerImpl() {}
	TimerImpl(const TimerImpl& timerImpl)
	{
		m_timer = timerImpl.m_timer;
	}

public:
	void Restart();
	double Elapsed() const;

private:
	timer m_timer;
};

void Timer::TimerImpl::Restart()
{
	m_timer.restart();
}

double Timer::TimerImpl::Elapsed() const
{
	return m_timer.elapsed();
}

Vdl::Timer::Timer()
{
	m_pImpl = new Timer::TimerImpl;
}

Vdl::Timer::Timer( const Timer& timer )
{
	m_pImpl = new Timer::TimerImpl;
	*m_pImpl = *timer.m_pImpl;
}

Vdl::Timer::~Timer()
{
	delete m_pImpl;
}

void Vdl::Timer::Restart()
{
	m_pImpl->Restart();
}

double Vdl::Timer::Elapsed() const
{
	return m_pImpl->Elapsed();
}
