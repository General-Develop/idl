#include <boost/smart_ptr/detail/atomic_count.hpp>
#include "boost/intrusive_ptr.hpp"

#include "intrusiveptr.h"

using namespace Vdl;

ReferenceCounter::ReferenceCounter() : m_pRefCount(new boost::detail::atomic_count(0))
{

}

ReferenceCounter::~ReferenceCounter() 
{
	delete m_pRefCount;
}

void ReferenceCounter::AddRef() 
{
	++(*m_pRefCount);
}

bool ReferenceCounter::Release() 
{
	return --(*m_pRefCount) == 0;
}
