#ifndef _LIST_ITERATOR_
#define _LIST_ITERATOR_

#include "listNode.h"
#include <memory>

template<class T, class  Ref, class Ptr>
struct List_Iterator {
	typedef List_Iterator<T, T&, T*> iterator;
	typedef List_Iterator<T, Ref, Ptr> self;
	
	typedef T 				value_type;
	typedef Ptr 			pointer;
	typedef Ref 			reference;
	typedef List_Node<T>*	link_type;
	typedef size_t 			size_type;
	typedef ptrdiff_t 		defference_type;

	link_type node;

#pragma region

	List_Iterator(link_type x) : node(x) {}
	List_Iterator() {}
	List_Iterator(const iterator& x) : node(x.node) {}

#pragma endregion

#pragma region

	bool operator==(const self& x) const { return node == x.node; }
	bool operator!=(const self& x) const { return node != x.node; }
	reference operator*() 		   const { return (*node).data;   }
	reference operator->()  	   const { return &(operator*()); }

	self& operator++() {
		node = (link_type)((*node).next);
		return *this;
	}

	self operator++(int) {
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--() {
		node = (link_type)((*node).prev);
		return *this;
	}

	self operator--(int) {
		self tmp = *this;
		--*this;
		return tmp;
	}

#pragma endregion
};

#endif