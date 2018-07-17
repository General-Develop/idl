#ifndef INTRUSIVE_PTR_H
#define INTRUSIVE_PTR_H

#include <assert.h>
#include "base.h"

namespace boost
{
	namespace detail
	{
		class atomic_count;
	} 
}

namespace Vdl
{
	class DECL_EXPORT ReferenceCounter
	{
	public:
		ReferenceCounter();
		virtual ~ReferenceCounter();  
		void AddRef();     
		bool Release(); 
	private:    
		ReferenceCounter& operator=(const ReferenceCounter&);   
	private:    
		ReferenceCounter(const ReferenceCounter&);
	private:
		boost::detail::atomic_count *m_pRefCount;
	};

	template <class T>
	class iIntrusivePtr 
	{
	public:
		iIntrusivePtr(): px( 0 )
		{
		}

		iIntrusivePtr( T * p): px( p )
		{
			if( px != 0 ) 
			{
				px->AddRef();
			}
		}

		iIntrusivePtr(iIntrusivePtr const & rhs): px( rhs.px )
		{
			if( px != 0 )
			{
				px->AddRef();
			}
		}

		~iIntrusivePtr()
		{
			if( px != 0 )
			{
				if (px->Release())
				{
					delete px;
				}
			}
		}
	public:
		T * get() const
		{
			return px;
		}

		T & operator*() const
		{
			assert( px != 0 );
			return *px;
		}

		T * operator->() const
		{
			assert( px != 0 );
			return px;
		}

		iIntrusivePtr & operator=(iIntrusivePtr const & rhs)
		{
			iIntrusivePtr(rhs).swap(*this);
			return *this;
		}

		iIntrusivePtr & operator=(T * rhs)
		{
			iIntrusivePtr(rhs).swap(*this);
			return *this;
		}

		operator bool () const
		{
			return px != 0;
		}

		// operator! is redundant, but some compilers need it
		bool operator! () const // never throws
		{
			return px == 0;
		}

		void swap(iIntrusivePtr & rhs)
		{
			T * tmp = px;
			px = rhs.px;
			rhs.px = tmp;
		}
	private:
		T * px;
	};

	template<class T, class U>
	inline bool operator==(iIntrusivePtr<T> const & a, iIntrusivePtr<U> const & b)
	{
		return a.get() == b.get();
	}

	template<class T, class U> 
	inline bool operator!=(iIntrusivePtr<T> const & a, iIntrusivePtr<U> const & b)
	{
		return a.get() != b.get();
	}

	template<class T, class U> 
	inline bool operator==(iIntrusivePtr<T> const & a, U * b)
	{
		return a.get() == b;
	}

	template<class T, class U>
	inline bool operator!=(iIntrusivePtr<T> const & a, U * b)
	{
		return a.get() != b;
	}

	template<class T, class U>
	inline bool operator==(T * a, iIntrusivePtr<U> const & b)
	{
		return a == b.get();
	}

	template<class T, class U>
	inline bool operator!=(T * a, iIntrusivePtr<U> const & b)
	{
		return a != b.get();
	}
}

#endif