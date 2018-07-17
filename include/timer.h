#ifndef TIMER_H
#define TIMER_H

#include "base.h"

namespace Vdl
{
	class DECL_EXPORT Timer
	{
	public:
		Timer();
		~Timer();
		Timer(const Timer& timer);

	public:
		void Restart();

		double Elapsed() const;

	private:
		class TimerImpl;
		TimerImpl* m_pImpl;
	};
}

#endif