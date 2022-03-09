#ifndef _DEQUE_ITERATOR_
#define _DEQUE_ITERATOR_

#include <memory>

template<class T, class Ref, class Ptr, size_t BufSiz>
struct Deque_Iterator {
#pragma region
	typedef T 									value_type;
	typedef Ptr 								pointer;
	typedef Ref 								reference;
	typedef size_t  							size_type;
	typedef ptrdiff_t 							difference_type;
	typedef T**  								map_pointer;
	typedef Deque_Iterator 						self;
	typedef Deque_Iterator<T, T&, T*, BufSiz> 	iterator;

	T* cur;
	T* first;
	T* last;
	map_pointer node;

#pragma endregion

#pragma region

	void set_node(map_pointer new_node) {
		node  = new_node;
		first = *new_node;
		last  = first + difference_type(buffer_size());
	}

#pragma endregion

#pragma region

	size_t buffer_size() {
		return BufSiz != 0 ? BufSiz : (sizeof(T) < 512 ? size_t(512 / sizeof(T)) : size_t(1));
	}

#pragma endregion

#pragma region
	
	#pragma region
	
	reference operator*() const { return *cur; }
	pointer operator->()  const { return &(operator*()); }

	#pragma endregion

	#pragma region

	self& operator++() {
		++cur;
		if(cur == last) {
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self& operator++(int) {
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--() {
		if(cur == first) {
			set_node(node - 1);
			cur = last;
		}
		-- cur;
		return *this;
	}

	self& operator--(int) {
		self tmp = *this;
		--*this;
		return tmp;
	}

	#pragma endregion

	#pragma region

	self& operator+=(difference_type n) {
		difference_type offset = n + (cur - first);
		if(offset >= 0 && offset < difference_type(buffer_size())) {
			cur += n;
		} else {
			difference_type node_offset = offset > 0 ?
			offset / difference_type(buffer_size()) :
			-difference_type((-offset - 1) / buffer_size()) - 1;
			cur = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}

	self operator+(difference_type n) const {
		self tmp = *this;
		return tmp += n;
	}

	self& operator-=(difference_type n) const {
		return *this += -n;
	}

	self operator-(difference_type n) const {
		self tmp = *this;
		return tmp -= n;
	}

	difference_type operator-(const self& x) {
		return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
	}

	reference operator[](difference_type n) const {
		return *(*this + n);
	}

	#pragma endregion

	#pragma region

	bool operator==(const self& x) const { return cur == x.cur; }
	bool operator!=(const self& x) const { return cur != x.cur; }
	bool operator< (const self& x) const { return (node == x.node) ? (cur < x.cur) : (node < x.node); }

	#pragma endregion

#pragma endregion

};

#endif