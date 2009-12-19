#ifndef adagio_shared_counter_h
#define adagio_shared_counter_h

#include <iostream>

#ifndef NULL
#define NULL 0
#endif

class base_counter
{
public:
	base_counter(bool strong = true)
	:is_strong(strong)
	,strong_count(new int(0))
	,weak_count(new int(0))
	{
		if(is_strong)
			++(*strong_count);
		else
			++(*weak_count);
	}
	
	base_counter(const base_counter& o, bool strong = true)
	:is_strong(strong)
	,strong_count(o.strong_count)
	,weak_count(o.weak_count)
	{
		if(is_strong && (*strong_count) == 0)
		{
			throw 0;
		}
		else if(is_strong)
		{
			++(*strong_count);
		}
		else
		{
			++(*weak_count);
		}
	}

	~base_counter()
	{
		if(is_strong)
			--(*strong_count);
		else
			--(*weak_count);
		if((*strong_count) == 0 && (*weak_count) == 0)
		{
			delete strong_count;
			delete weak_count;
		}
	}

	bool operator==(base_counter const & o)
	{
		return (strong_count == o.strong_count);
	}

	base_counter& operator=(base_counter const & o)
	{
		//Clean up old data
		if(is_strong)
			--(*strong_count);
		else
			--(*weak_count);
		if((*strong_count) == 0 && (*weak_count) == 0)
		{
			delete strong_count;
			delete weak_count;
		}
		//Set up new data
		is_strong = o.is_strong;
		strong_count = o.strong_count;
		weak_count = o.weak_count;
		if(is_strong && (*strong_count) == 0)
			throw 0;
		else if(is_strong)
			++(*strong_count);
		else
			++(*weak_count);
		return *this;
	}
	
	long get_strong_count() const
	{
		return *strong_count;
	}
	long get_weak_count() const
	{
		return *weak_count;
	}
private:
	bool is_strong;
	int* strong_count;
	int* weak_count;
};

#endif
