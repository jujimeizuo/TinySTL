#ifndef _ALLOC_
#define _ALLOC_
 
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>



#pragma region
// 内存分配
template<class T>
inline T* _allocate(ptrdiff_t size, T*) {
	std::set_new_handler(0);
	T* tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
	if (tmp == 0) {
		std::cerr << "out of memory" << std::endl;
		exit(1);
	}
	return tmp;
}

// 内存释放
template<class T>
inline void _deallocate(T* buffer) {
	::operator delete(buffer);
}

// 元素构造
template<class T1, class  T2>
inline void _construct(T1* p, const T2& value) {
	new(p) T1(value);
}

// 元素析构
template<class T>
inline void _destroy(T* ptr) {
	ptr -> ~T();
}
#pragma endregion

#pragma region
template<class T>
class Allocator {
public:
	typedef T			value_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;

	template<class U>
	struct rebind {
		typedef Allocator<U> other;
	};

	static pointer allocate(size_type n, const void* hint = 0) {
		return _allocate((difference_type)n, (pointer)0);
	}

	static void deallocate(pointer p, size_type n) {
		_deallocate(p);
	}

	static void deallocate(void* p) {
		_deallocate(p);
	}

	void construct(pointer p, const T& value) {
		_construct(p, value);
	}

	void destroy(pointer p) {
		_destroy(p);
	}

	pointer address(reference x) {
		return (pointer)&x;
	}

	const_pointer const_address(const_reference x) {
		return (const_pointer)&x;
	}

	size_type max_size() const {
		return size_type(UINT_MAX / sizeof(T));
	}
};
#pragma endregion

#pragma region
template<class T, class Alloc = Allocator<T>>
class simple_alloc {
public:
	static T* allocate(size_t n) {
		return 0 == n ? 0 : (T*)Alloc::allocate(n*sizeof(T));
	}

	static T* allocate(void) {
		return (T*)Alloc::allocate(sizeof(T));
	}

	static void deallocate(T* p, size_t n) {
		if (0 != n)Alloc::deallocate(p, n*sizeof(T));
	}

	static void deallocate(void* p) {
		Alloc::deallocate(p);
	}
};
#pragma endregion

#endif