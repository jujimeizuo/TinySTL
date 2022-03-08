#ifndef _VECTOR_
#define _VECTOR_

#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"
#include <memory>

template<class T, class Alloc = Allocator<T>>
class Vector {
public :
	typedef T 			value_type;
	typedef T* 			pointer;
	typedef T*  		iterator;
	typedef T& 			reference;
	typedef size_t  	size_type;
	typedef ptrdiff_t 	difference_type;

protected :
	typedef simple_alloc<value_type, Alloc> data_allocator; // 定义空间配置器
	iterator start;
	iterator finish;
	iterator end_of_storage;

	void insert_aux(iterator position, const T& value) {
		if(finish != end_of_storage) {
			construct(finish, *(finish - 1));
			++finish;
			T x_copy = value;
			std::copy_backward(position, finish - 2, finish - 1);
			*position = x_copy;
		} else {
			ptrdiff_t old_size = size();
			const size_type len = old_size != 0 ? old_size * 2 : 1;
			iterator new_start  = data_allocator::allocate(len);
			iterator new_finish = new_start;
			try {
				new_finish = std::uninitialized_copy(start, position, new_start);
				construct(new_finish, value);
				++new_finish;
			} catch (std::exception ex) {
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, len);
				throw ;
			}
			destroy(begin(), end());
			deallocate();
			start  = new_start;
			finish = new_finish;
			end_of_storage = new_start + len;
		}
	}

	#pragma region // 释放内存和析构元素
	
	void deallocate() {
		if(start) {
			data_allocator::deallocate(start, end_of_storage - start);
		}
	}

	#pragma endregion

	#pragma region // 分配内存和构造元素

	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

	iterator allocate_and_fill(size_type n, const T& x) {
		iterator result = data_allocator::allocate(n);
		iterator cur = result;
		for( ;n > 0; --n, ++cur) {
			construct(&*cur, x);
		}
		return result;
	}

	#pragma endregion

public :
	#pragma region // 对Vector的读操作
	
	iterator begin() 					{ return start; }
	iterator end()   					{ return finish; }
	size_type size() 					{ return size_type(int(end() - begin())); }
	size_type capacity() const  		{ return size_type(end_of_storage - start); }
	bool empty() const 					{ return begin() == end(); }
	reference operator[](size_type n)	{ return *(begin() + n); }
	reference front() 					{ return *begin(); }
	reference back() 					{ return *(end() - 1); }

	#pragma endregion

	#pragma region // 构造函数
	Vector() : start(0), finish(0), end_of_storage(0) {}
	Vector(size_type n, const T& value) { fill_initialize(n, value); }
	Vector(int n, const T& value) 		{ fill_initialize(n, value); }
	Vector(long n, const T& value) 		{ fill_initialize(n, value); }
	explicit Vector(size_type n) 		{ fill_initialize(n, T()); }

	#pragma endregion

	#pragma region

	~Vector() {
		destroy(start, finish);
		deallocate();
	}

	#pragma endregion

	#pragma region // 对Vector的写操作

	void pop_back() {
		--finish;
		destroy(finish);
	}

	void push_back(const T& x) {
		if(finish != end_of_storage) {
			construct(finish, x);
			++finish;
		} else {
			insert_aux(end(), x);
		}
	}

	iterator erase(iterator first, iterator last) {
		iterator i = std::copy(last, finish, first);
		destroy(i, finish);
		finish = finish - (last - first);
		return first;
	}

	iterator erase(iterator position) {
		if(position + 1 != end()) {
			std::copy(position + 1, finish, position);
		}
		-- finish;
		destroy(finish);
		return position;
	}

	#pragma endregion
};

#endif