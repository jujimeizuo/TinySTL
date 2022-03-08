#ifndef _LIST_
#define _LIST_

#include "listNode.h"
#include "listIterator.h"
#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"

template<class T, class Alloc = Allocator<T>>
class List {
protected :
	typedef List_Node<T> 								list_node;

public :
	typedef list_node* 									link_type;
	typedef size_t  									size_type;
	typedef typename List_Iterator<T, T&, T*>::iterator	iterator;

#pragma region // 内存控制、节点构造与析构

protected :
	link_type node;
	typedef simple_alloc<list_node, Alloc> 				list_node_allocator;
	link_type get_node() 		{ return list_node_allocator::allocate(); }
	void put_node(link_type p) 	{ list_node_allocator::deallocate(p); }

	link_type create_node(const T& x) {
		link_type p = get_node();
		construct(&p -> data, x);
		return p;
	}

	void destroy_node(link_type p) {
		destroy(&p -> data);
		put_node(p);
	}

public :
	List() { empty_initialize(); }

protected :
	void empty_initialize() {
		node = get_node();
		node -> next = node;
		node -> prev = node;
	}

#pragma endregion

public :

#pragma region

	iterator begin() const { return (link_type)((*node).next); }
	
	iterator end()   const { return node; }
	
	bool empty() 	 const { return node -> next == node; }
	
	size_type size() const {
		size_type result = 0;
		iterator first = begin();
		iterator last  = end();
		while(first != last) {
			++first;
			++result;
		}
		return result;
	}

#pragma endregion

#pragma region

	void push_back(const T& x) 	{ insert(end(), x); }
	
	void push_front(const T& x)	{ insert(begin(), x); }
	
	iterator insert(iterator position, const T& x) {
		link_type tmp = create_node(x);
		tmp -> next = position.node;
		tmp -> prev = position.node -> prev;
		(link_type(position.node -> prev)) -> next = tmp;
		position.node -> prev = tmp;
		return tmp;
	}

#pragma endregion

#pragma region

	iterator erase(iterator position) {
		link_type next_node = link_type(position.node -> next);
		link_type prev_node = link_type(position.node -> prev);
		prev_node -> next   = next_node;
		next_node -> prev   = prev_node;
		destroy_node(position.node);
		return iterator(next_node);
	}

	void pop_front() { erase(begin()); }
	
	void pop_back()  { 
		iterator tmp = end();
		erase(--tmp);
	}

	void clear() {
		link_type cur = (link_type)node -> next;
		while(cur != node) {
			iterator tmp = cur;
			cur = (link_type) cur -> next;
			destroy_node(tmp.node);
		}
		node -> next = node;
		node -> prev = node;
	}

	void remove(const T& value) {
		iterator first = begin();
		iterator last  = end();
		while(first != last) {
			iterator next = first;
			++next;
			if(*first == value) erase(first);
			first = next;
		}
	}

	void unique(const T& value) {
		iterator first = begin();
		iterator last  = end();
		if(first == last) return ;
		iterator next = first;
		while(++next != last) {
			if(*first == *next) {
				erase(next);
			} else {
				first = next;
			}
			next = first;
		}
	}

#pragma endregion
};

#endif