#ifndef THREAD_IMPL_H_
#define THREAD_IMPL_H_

#include <string>

#include <boost/asio.hpp>
#include <boost/smart_ptr/detail/atomic_count.hpp>
#include <boost/thread/condition.hpp>

#include "intrusiveptr.h"

namespace boost
{
	class thread;
}

namespace Vdl
{
	class MethodRequest;
	typedef iIntrusivePtr<MethodRequest> MethodRequestPtr;

	class Thread;

	class ThreadImpl
	{
	public:
		ThreadImpl(Thread& thread, const std::string& name);
		~ThreadImpl();

	public:
		virtual bool Start(boost::uint32_t second);
		virtual void Stop();
		virtual void TimeOut();

	public:
		bool PushMethod(MethodRequestPtr methodPtr);

		const std::string& GetName() const;
		bool IsRunning() const;

	public:
		void Svc();

	private:
		static void S_svc(ThreadImpl* pImpl);
		static void S_TimeOutJob(ThreadImpl* pImpl);
		static void S_HandleJob(ThreadImpl* pImpl,MethodRequestPtr methodPtr);

		static void S_TimeOut(ThreadImpl* pImpl);

		static std::string GetMethodName(const MethodRequest& method);

	private:
		Thread& m_thread;
		const std::string m_taskName;

		boost::uint32_t m_interval;

		boost::asio::io_service m_io;
		boost::asio::strand<boost::asio::io_service::executor_type> m_strand;

		boost::detail::atomic_count m_methodCount;

		boost::thread* m_pThread;
		boost::asio::deadline_timer* m_pTimer;

		boost::mutex m_mutex;
		boost::condition m_conditon;
		bool m_isActived;
		bool m_isStarted;
		bool m_isStarteSuccess;
	};
}

#endif