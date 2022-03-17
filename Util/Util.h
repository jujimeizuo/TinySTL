#ifndef _UTIL
#define _UTIL

template<class T1, class T2>
struct Pair {
	typedef T1   	first_type;
	typedef T2 		second_type;
	T1 first;
	T2 second;

	Pair() : first(T1()), second(T2()) {}
	Pair(const T1& a, const T2& b) : first(a), second(b) {}
};

template<class T>
void swap(T& a, T& b) {
	T c(a); a = b; b = c;
}

void * __cdecl memove(void * dst, const void * src, size_t count) {
	void *ret = dst;
	if(dst <= src || (char *) dst >= ((char *) src + count)) {
		while(count--) {
			*(char *) dst = *(char *) src;
			dst = (char *) dst + 1;
			src = (char *) src + 1;
		}
	}
	return (ret);
}

void * __cdecl memcpy(void * dst, const void * src, size_t count) {
	void * ret = dst;
	while(count--) {
		*(char *) dst = *(char *) src;
		dst = (char *) dst + 1;
		src = (char *) src + 1;
	}
	return (ret);
}

#endif