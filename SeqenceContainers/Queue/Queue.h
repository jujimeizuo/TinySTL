#ifndef _QUEUE_
#define _QUEUE_

#include "../Deque/Deque.h"
#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"

template<class T, class sequence = Deque<T>>
class Queue {
public :
	typedef typename sequence::value_type		value_type;
	typedef typename sequence::size_type		size_type;
	typedef typename sequence::reference		reference;

protected :
	sequence s;

public :
	Queue() : s() {}
	bool empty() const  { return s.empty(); }
	size_type size() 	{ return s.size(); }
	reference front() 	{ return s.front(); }
	reference back() 	{ return s.back(); }
	void push(const value_type& value) { s.push_back(value); }
	void pop() 			{ s.pop_front(); }

};

#endif