#ifndef adagio_shared_ptr_h
#define adagio_shared_ptr_h

#include <iostream>
#include "shared_counter.h"

#ifndef NULL
#define NULL 0
#endif

template<class T>
class shared_ptr
{
public:
	shared_ptr()
	:instance(NULL)
	{
	}

	shared_ptr(T* i)
	:instance(i)
	{
	}

	shared_ptr(const shared_ptr<T>& sp)
	:counter(sp.counter)
	,instance(sp.instance)
	{
		if(instance == NULL)
		{
			counter = base_counter();
		}
	}

	template<class Y>
	shared_ptr(shared_ptr<Y> const & r)
	:counter(r.counter)
	,instance(dynamic_cast<T*>(r.instance))
	{
		if(instance == NULL)
		{
			counter = base_counter();
		}
	}

	~shared_ptr()
	{
		if(instance && counter.get_strong_count() == 1)
		{
			delete instance;
		}
	}

	void reset(T* i = NULL)
	{
		if(instance && counter.get_strong_count() == 1)
		{
			delete instance;
		}
		instance = i;
		counter = base_counter();
	}

	void reset(shared_ptr<T> sp)
	{
		counter = sp.counter;
		instance = sp.instance;
	}
	
	bool operator==(shared_ptr<T> const & o)
	{
		return (instance == o.instance);
	}

	shared_ptr<T> & operator=(shared_ptr<T> const & r)
	{
		reset(r);
		return *this;
	}

	T& operator*() const
	{
		return *instance;
	}
	
	T* operator->() const
	{
		return instance;
	}

	bool operator! () const // never throws
    {
        return instance == NULL;
    }

	T * get() const // never throws
    {
        return instance;
    }
	
	long use_count() const
	{
		return counter.get_strong_count();
	}

public: //No worries, even boost has public members!
	base_counter counter;
	T* instance;
};

template<class T, class U>
shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const & r)
{
	return shared_ptr<T>(r);
}

#endif
