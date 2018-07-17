#ifndef THREAD_H_
#define THREAD_H_

#include <string>

#include "intrusiveptr.h"
#include "base.h"

namespace Vdl
{
	class MethodRequest;
	typedef iIntrusivePtr<MethodRequest> MethodRequestPtr;

	class DECL_EXPORT Thread
	{
	public:
		Thread(const std::string& name);
		~Thread();

	public:
		virtual bool Start(boost::uint32_t second = 1);
		virtual void Stop();
		virtual void TimeOut();
		
	public:
		bool PushMethod(MethodRequestPtr methodPtr);

		const std::string& GetName() const;
		bool IsRunning() const;

	private:
		virtual bool InitializeTask();
		virtual void ExitTask();
		virtual bool InitializeThread();
		virtual void ExitThread();
		virtual void Svc();

	private:
		friend class ThreadImpl;
		ThreadImpl* m_pThreadImpl;
	};
}

#endif