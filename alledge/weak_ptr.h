#ifndef adagio_weak_ptr_h
#define adagio_weak_ptr_h

#include <iostream>
#include "shared_counter.h"

#ifndef NULL
#define NULL 0
#endif

template<class T>
class weak_ptr
{
public:
	weak_ptr()
	:counter(base_counter(false))
	,instance(NULL)
	{
	}

	weak_ptr(const shared_ptr<T>& sp)
	:counter(base_counter(sp.counter, false))
	,instance(sp.instance)
	{
		if(instance == NULL)
			counter = base_counter(false);
	}

	weak_ptr(const weak_ptr<T>& sp)
	:counter(base_counter(sp.counter, false))
	,instance(sp.instance)
	{
		if(instance == NULL)
			counter = base_counter(false);
	}

	weak_ptr& operator=(const weak_ptr<T>& sp)
	{
		counter = base_counter(sp.counter, false);
		instance = sp.instance;
		if(instance == NULL)
			counter = base_counter(false);
		return *this;
	}

	weak_ptr& operator=(const shared_ptr<T>& sp)
	{
		counter = base_counter(sp.counter, false);
		instance = sp.instance;
		if(instance == NULL)
			counter = base_counter(false);
		return *this;
	}
	
	void reset()
	{
		counter = base_counter(false);
		instance = NULL;
	}

	shared_ptr<T> lock()
	{
		if(counter.get_strong_count() == 0)
		{
			return shared_ptr<T>();
		}
		shared_ptr<T> r;
		try
		{
			r.counter = base_counter(counter, true);
			r.instance = instance;
		}
		catch(int)
		{
			r.reset();
		}
		return r;
	}
private:
	base_counter counter;
	T* instance;
};

#endif
