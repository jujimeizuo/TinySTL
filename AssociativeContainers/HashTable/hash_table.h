#ifndef _HASH_TABLE_
#define _HASH_TABLE_

#include "../../SeqenceContainers/Vector/Vector.h"
#include "hash_table_iterator.h"
#include "../../Util/Util.h"
#include <algorithm>

template<class value, class key, class hashFun, class extractKey, class equalKey, class Alloc = Allocator<key>>
class hash_table {

#pragma region

public :
	typedef hashFun 																hasher;
	typedef equalKey 																key_equal;
	typedef size_t    																size_type;
	typedef value  	  																value_type;
	typedef key   	  																key_type;
	typedef hash_table_iterator<value, key, hasher, extractKey, equalKey, Alloc>   	iterator;

private :
	hasher 									hash;
	key_equal 								equals;
	extractKey 								get_key;
	typedef hash_table_node<value> 			node;
	typedef simple_alloc<node, Alloc> 		node_allocator;

public :
	size_type 								num_elements;
	Vector<node*, Alloc> 					buckets;

#pragma endregion

#pragma region

private :
	node* new_node(const value_type& obj) {
		node* n = node_allocator::allocate();
		n -> next = 0;
		construct(&n -> val, obj);
		return n;
	}

	void delete_node(node* n) {
		destroy(&n -> val);
		node_allocator::deallocate(n);
	}

	inline unsigned long __stl_next_prime(unsigned long n) {
		const unsigned long* first = __stl_prime_list;
		const unsigned long* last = __stl_prime_list + __stl_prime_primes;
		const unsigned long* pos = std::lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}

	size_type next_size(size_type n) { return __stl_next_prime(n); }

	void initialize_buckets(size_type n) {
		const size_type n_buckets = next_size(n);
		buckets.insert(buckets.end(), n_buckets, 0);
	}

public :
	hash_table(size_type n, const hashFun& hf, const equalKey& eql) : hash(hf), equals(eql), get_key(extractKey()), num_elements(0) {
		initialize_buckets(n);
	}

#pragma endregion

#pragma region

	#pragma region

public :
	size_type bucket_count() { return buckets.size(); }
	
	size_type max_bucket_count() const { return __stl_prime_list[__stl_num_primes - 1]; }
	
	size_type size() const { return num_elements; }
	
	iterator begin() {
		for(size_type n = 0;n < buckets.size(); ++n) {
			if(buckets[n]) {
				return iterator(buckets[n], this);
			}
		}
		return end();
	}
	
	iterator end() { return iterator(0, this); }
	
	size_type elems_in_bucket(size_type num) {
		size_type result = 0;
		node* cur = buckets[num];
		for(node* cur = buckets[num]; cur; cur = cur -> next) {
			result += 1;
		}
		return result;
	}

	iterator find(const key_type& key) {
		size_type n = bkt_num(key);
		node* first;
		for(first = buckets[n]; first && !equals(get_key(first -> val), key); first = first -> next) {}
			return iterator(first, this);
	}
	size_type count(const key_type& key) {
		size_type n = bkt_num(key);
		size_type result = 0;
		for(const node* cur = buckets[n]; cur; cur = cur -> next) {
			if(equals(get_key(cur -> val), key)) {
				++result;
			}
		}
		return result;
	}

	#pragma endregion

	#pragma region

		#pragma region
public :
	Pair<iterator, bool> insert_unique(const value_type& obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}

	iterator insert_equal(const value_type& obj) {
		resize(num_elements + 1);
		return insert_unique_noresize(obj);
	}

		#pragma endregion

		#pragma region

private :
	void resize(size_type num_elements_hint) {
		const size_type old_n = buckets.size();
		if(num_elements_hint > old_n) {
			const size_type n = next_size(num_elements_hint);
			if(n > old_n) {
				Vector<node*, Alloc> tmp(n, (node*) n);
				for(size_type bucket = 0;bucket < old_n; ++bucket) {
					node* first = buckets[bucket];
					while(first) {
						size_type new_bucket = bkt_num(first -> val, n);
						buckets[bucket] = first -> next;
						first -> next = tmp[new_bucket];
						tmp[new_bucket] = first;
						first = buckets[bucket];
					}
				}
				buckets.swap(tmp);
			}
		}
	}

	Pair<iterator, bool> insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		for(node* cur = first; cur; cur = cur -> next) {
			if(equals(get_key(cur -> val), get_key(obj))) {
				return Pair<iterator, bool>(iterator(cur, this), false);
			}
		}
		node* tmp = new_node(obj);
		tmp -> next = first;
		buckets[n] = tmp;
		++num_elements;
		return Pair<iterator, bool>(iterator(tmp, this), true);
	}

	iterator insert_unique_noresize(const value_type& obj) {
		const size_type n = bkt_num(obj);
		node* first = buckets[n];
		for(node* cur = first; cur; cur = cur -> next) {
			if(equals(get_key(cur -> val), get_key(obj))) {
				node* tmp = new_node(obj);
				tmp -> next = cur -> next;
				cur -> next = tmp;
				++num_elements;
				return iterator(tmp, this);
			}
		}
		node* tmp = new_node(obj);
		tmp -> next = first;
		buckets[n] = tmp;
		return iterator(tmp, this);
	}

		#pragma endregion

	#pragma endregion

#pragma endregion

#pragma region

public :
	size_type bkt_num(const value_type& obj, size_type n) {
		return bkt_num_key(get_key(obj), n);
	}

	size_type bkt_num(const value_type& obj) {
		return bkt_num_key(get_key(obj));
	}

	size_type bkt_num_key(const key_type& key) {
		return bkt_num_key(key, buckets.size());
	}

	size_type bkt_num_key(const key_type& key, size_type n) {
		return hash(key) % n;
	}

#pragma endregion

};

static const int __stl_prime_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] = {
	53, 97, 193, 389, 769,
	1543, 3079, 6151, 12289, 24593,
	49157, 98317, 196613, 393241, 786433,
	1572869, 3145739, 6291469, 12582917, 25165843,
	50331653, 100663319, 201326611, 402653189, 805306457,
	1610612741, 3221225473ul, 4294967291ul
};

#endif

























