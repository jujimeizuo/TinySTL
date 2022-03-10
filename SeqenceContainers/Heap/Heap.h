#ifndef _HEAP_
#define _HEAP_

#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"
#include <memory>

template<class T, class Alloc = Allocator<T>>
class Heap {
public :
	typedef T 						value_type;
	typedef value_type*  			pointer;
	typedef value_type*  			iterator;
	typedef value_type& 			reference;
	typedef size_t 					size_type;
	typedef ptrdiff_t 				difference_type;

protected :
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;
	iterator finish;
	iterator end_of_storage;

#pragma region

	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		iterator cur = result;
		for( ; n > 0; --n, ++cur) {
			construct(&*cur, x);
		}
		return result;
	}

	void deallocate() {
		if(start) {
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}

#pragma endregion

public :

#pragma region

	Heap() : start(0), finish(0), end_of_storage(0) {}
	Heap(size_type n, const T& value) 	 { fill_initialize(n, value); }
	Heap(int n, const T& value)			 { fill_initialize(n, value); }
	Heap(long n, const T& value) 		 { fill_initialize(n, value); }
	explicit Heap(size_type n) 			 { fill_initialize(n, T()); }
	
	~Heap() {
		destroy(start, finish);
		deallocate();
	}

#pragma endregion

#pragma region

	iterator begin() { return start; }
	iterator end()   { return finish; }
	size_type size() { return size_type(int(end() - begin())); }

#pragma endregion

#pragma region

	void push_back(const T& x) {
		if(finish != end_of_storage) {
			construct(finish, x);
			++finish;
		} else {
			insert_aux(end(), x);
		}
	}

	void pop_back() {
		--finish;
		destroy(finish);
	}

	inline void make_heap(iterator first, iterator last) {
		if(last < first < 2) return ;
		difference_type len = last - first;
		difference_type parent = len / 2;
		while(true) {
			_adjust_heap(first, parent, len, T(*(first + parent)));
			if(parent == 0) return ;
			--parent;
		}
	}

	inline void push_heap(iterator first, iterator last) {
		difference_type holeIndex = last - first - 1;
		difference_type topIndex = 0;
		T value = T(*(last - 1));
		_push_heap(first, holeIndex, topIndex, value);
	}

	inline void pop_heap(iterator first, iterator last) {
		__pop_heap(first, last - 1, last - 1, T(*(last - 1)));
	}

	inline void sort_heap(iterator first, iterator last) {
		while(last - first > 1) {
			pop_heap(first, last--);
		}
	}

#pragma endregion

protected :

#pragma region

	void insert_aux(iterator position, const T& value) {
		if(finish != end_of_storage) {
			construct(finish, *(finish - 1));
			++finish;
			T value_copy = value;
			std::copy_backward(position, finish - 2, finish - 1);;
			*position = value_copy;
		} else {
			ptrdiff_t old_size = size();
			const size_type len = old_size != 0 ? 2 * old_size : 1;
			iterator new_start = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = std::uninitialized_copy(start, position, new_start);
				construct(new_finish, value);
				++new_finish;
			} catch(std::exception ex) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw;
			}
			destroy(begin(), end());
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	void __pop_heap(iterator first, iterator last, iterator result, T value) {
		*result = *first;;
		_adjust_heap(first, difference_type(0), difference_type(last - first), value);
	}

	void _adjust_heap(iterator first, difference_type holeIndex, difference_type len, T value) {
		difference_type topIndex = holeIndex;
		difference_type secondChild = 2 * holeIndex + 2;
		while(secondChild < len) {
			if(*(first + secondChild) < *(first + (secondChild - 1))) {
				--secondChild;
			}
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = 2 * (secondChild + 1);
		}
		if(secondChild == len) {
			*(first + holeIndex) = *(first + (secondChild - 1));
			holeIndex = secondChild - 1;
		}
		_push_heap(first, holeIndex, topIndex, value);
	}

	void _push_heap(iterator first, difference_type holeIndex, difference_type topIndex, T value) {
		difference_type parent = (holeIndex - 1) / 2;
		while(holeIndex > topIndex && *(first + parent) < value) {
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = value;
	}

#pragma endregion

};

#endif













