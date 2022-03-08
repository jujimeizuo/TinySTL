#ifndef _GLOBAL_CONSTRUCT_
#define _GLOBAL_CONSTRUCT_

// #include <stdafx>
#include <new>
#include <type_traits>

#pragma region // 统一的构造析构函数

template<class T1, class T2>
inline void construct(T1* p, const T2& value) {
	new (p)T1(value);
}

template<class T>
inline void destroy(T* pointer) {
	pointer -> ~T();
}

template<class ForwordIterator>
inline void destroy(ForwordIterator first, ForwordIterator last) {
	for( ; first < last; ++first) {
		destroy(&*first);
	}
}

#pragma endregion

#endif