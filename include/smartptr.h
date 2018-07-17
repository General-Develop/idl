#ifndef SMART_PTR_H
#define SMART_PTR_H

#include <boost/shared_ptr.hpp>

namespace Vdl
{
	template <class T>
	class iSmartPtr : public boost::shared_ptr<T>
	{
	public:
		iSmartPtr() : boost::shared_ptr<T>((T*)NULL)
		{

		}

		iSmartPtr(T * t) : boost::shared_ptr<T>(t)
		{

		}

		iSmartPtr& operator=(T * t)
		{
			if (boost::shared_ptr<T>::get() != t)
			{
				boost::shared_ptr<T>::operator=(boost::shared_ptr<T>(t)); 
			}
			return *this; 
		}

		iSmartPtr& operator=(const iSmartPtr& t)
		{
			if (boost::shared_ptr<T>::get() != t.get())
			{
				boost::shared_ptr<T>::operator=(t);
			}
			return *this;
		}

		T* operator->() const { return boost::shared_ptr<T>::operator->();}
	};
}

#endif