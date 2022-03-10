#ifndef _STACK_
#define _STACK_

#include "../Deque/Deque.h"
#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"

template<class T, class sequeue = Deque<T>>
class Stack {
public :
	typedef typename sequeue::value_type  value_type;
	typedef typename sequeue::size_type   size_type;
	typedef typename sequeue::reference   reference;

protected :
	sequeue s;

public :
	Stack() : s() {}
	bool empty() const  { return s.empty(); }
	size_type size() 	{ return s.size(); }
	reference top() 	{ return s.back(); }
	void push(const value_type& value) { s.push_back(value); }
	void pop() 			{ s.pop_back(); }
};


#endif