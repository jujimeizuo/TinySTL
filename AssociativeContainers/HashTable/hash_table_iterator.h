#ifdef _HASH_TABLE_ITERATOR_
#define _HASH_TABLE_ITERATOR_

#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"
#include "hash_table_node.h"
#include "hash_table.h"
#include <memory>

template<class value, class key, class hashFun, class extractKey, class equalKey, class Alloc = Allocator<key>>
class hash_table;

template<class value, class key, class hashFun, class extractKey, class equalKey, class Alloc = Allocator<key>>
struct hash_table_iterator {

#pragma region

	typedef hash_table_iterator<value, key, hashFun, extractKey, equalKey, Alloc>  				iterator;
	typedef hash_table_node<value> 																node;
	typedef hash_table<value, key, hashFun, extractKey, equalKey, Alloc>  						hash_table;

	typedef std::forward_iterator_tag 															iterator_category;
	typedef value   																			value_type;
	typedef ptrdiff_t 																			difference_type;
	typedef size_t  																			size_type;
	typedef value& 																				reference;
	typedef value* 																				pointer;

	node* 																						cur;
	hash_table* 																				it;

#pragma endregion

#pragma region

	hash_table_iterator(node* n, hash_table* tab) : cur(n), ht(tab) {}
	hash_table_iterator() {}

#pragma endregion

#pragma region

	reference operator*()   const{ return cur -> val; }
	pointer operator->()    const{ return &(operator*()); }
	iterator& operator++() {
		const node* old = cur;
		cur = cur -> next;
		if(!cur) {
			size_type bucket = ht -> bkt_num(old -> val);
			while(!cur && ++bucket < ht -> bucket.size()) {
				cur = ht -> buckets[bucket];
			}
			return *this;
		}
	}
	iterator& operator++(int) {
		iterator tmp = *this;
		++*this;
		return tmp;
	}

#pragma endregion

};

#endif