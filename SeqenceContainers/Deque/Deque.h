#ifndef _DEQUE_
#define _DEQUE_

#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"
#include "dequeIterator.h"
#include <algorithm>

template<class T, class Alloc = Allocator<T>, size_t BufSiz = 0>
class Deque {
#pragma region

public :
	typedef T  														value_type;
	typedef value_type* 											pointer;
	typedef value_type&  											reference;
	typedef size_t  												size_type;
	typedef ptrdiff_t  												difference_type;
	typedef typename Deque_Iterator<T, T&, T*, BufSiz>::iterator 	iterator;

protected :
	typedef pointer* map_pointer;

protected :
	iterator 		start;
	iterator 		finish;
	map_pointer 	map;
	size_type 		map_size;

#pragma endregion

#pragma region

protected :
	size_t buffer_size() {
		return BufSiz != 0 ? BufSiz : (sizeof(T) < 512 ? size_t(512 / sizeof(T)) : size_t(1));
	}

#pragma endregion

#pragma region

protected :
	typedef simple_alloc<value_type, Alloc> data_allocator;
	typedef simple_alloc<pointer, Alloc>    map_allocator;

public :
	Deque(int n, const value_type& value) : start(), finish(), map(0), map_size(0) {
		fill_initialize(n, value);
	}

	Deque() : start(), finish(), map(0), map_size(0) {
		fill_initialize(0, 0);
	}

protected :
	void fill_initialize(size_type n, const value_type& value) {
		create_map_and_nodes(n);
		map_pointer cur;
		for(cur = start.node; cur < finish.node; ++cur) {
			std::uninitialized_fill(*cur, *cur + buffer_size(), value);
		}
		std::uninitialized_fill(finish.first, finish.cur, value);
	}

	void create_map_and_nodes(size_type num_elements) {
		size_type num_nodes = num_elements / buffer_size() + 1;
		map_size = std::max(initial_map_size(), num_nodes + 2);
		map = map_allocator::allocate(map_size);
		map_pointer nstart = map + (map_size - num_nodes) / 2;
		map_pointer nfinish = nstart + num_nodes - 1;
		map_pointer cur;
		for(cur = nstart; cur <= nfinish; ++ cur) {
			*cur = allocate_node();
		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + num_elements % buffer_size();
	}

	size_type initial_map_size() { return (size_type)8; }

	pointer allocate_node() {
		return data_allocator::allocate(buffer_size() / sizeof(T));
	}

	void deallocate_node(void* node) {
		data_allocator::deallocate(node);
	}

#pragma endregion

#pragma region

public :
	iterator begin() { return start; }
	iterator end()   { return finish;}
	reference operator[](size_type n) {
		return start[difference_type(n)];
	}
	reference front() { return *start; }
	reference back()  {
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}

	size_type size() {
		return finish - start;
	}

	size_type max_size() const {
		return finish - start;
	}

	bool empty() const {
		return finish == start;
	}

#pragma endregion

#pragma region

public :
	void push_back(const value_type& value) {
		if(finish.cur != finish.last - 1) {
			construct(finish.cur, value);
			++finish.cur;
		} else {
			push_back_aux(value);
		}
	}

	void push_front(const value_type& value) {
		if(start.cur != start.first) {
			construct(start.cur - 1, value);
			--start.cur;
		} else {
			push_front_aux(value);
		}
	}

	void pop_back() {
		if(finish.cur != finish.first) {
			--finish.cur;
			destroy(finish.cur);
		} else {
			pop_back_aux();
		}
	}

	void pop_front() {
		if(start.cur != start.last - 1) {
			destroy(start.cur);
			++start.cur;
		} else {
			pop_front_aux();
		}
	}

	void clear() {
		for(map_pointer node = start.node + 1; node < finish.node; ++node) {
			destroy(*node, *node + buffer_size());
			data_allocator::deallocate(*node, buffer_size());
		}
		if(start.node != finish.node) {
			destroy(start.cur, start.last);
			destroy(finish.first, finish.last);
			data_allocator::deallocate(finish.first, buffer_size());
		} else {
			destroy(start.cur, finish.cur);
		}
		finish = start;
	}

protected :
	void push_back_aux(const value_type& value) {
		value_type value_copy = value;
		*(finish.node + 1) = allocate_node();
		construct(finish.cur, value_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	void push_front_aux(const value_type& value) {
		value_type value_copy = value;
		*(start.node - 1) = allocate_node();
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, value_copy);
	}

	void pop_back_aux() {
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	void pop_front_aux() {
		destroy(start.cur);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}

#pragma endregion

};

#endif