#ifndef _LIST_NODE_
#define _LIST_NODE_

template<typename T>
struct List_Node {
	typedef void* void_pointer;
	void_pointer prev;
	void_pointer next;
	T data;
};

#endif