#ifndef _MULTI_SET_
#define _MULTI_SET_

#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"
#include "../RB-Tree/RBTree.h"

template<class key, class compare = std::less<key>, class Alloc = Allocator<key>>
class Multiset {
#pragma region

private :
	typedef key    																				key_type;
	typedef key  																				value_type;
	typedef compare  																			key_compare;
	typedef compare   																			value_compare;
	typedef RBTree<key_type, value_type, std::identity<value_type>, key_compare, Alloc>      	rep_type;
	rep_type 																					t;

public :
	typedef typename rep_type::const_pointer  													pointer;
	typedef typename rep_type::const_pointer   													const_pointer;
	typedef typename rep_type::const_reference 													reference;
	typedef typename rep_type::const_reference  												const_reference;
	typedef typename rep_type::iterator   														iterator;
	typedef typename rep_type::size_type  														size_type;
	typedef typename rep_type::difference_type 													difference_type;

#pragma endregion

#pragma region

public :
	Multiset() : t(compare()) {}
	Multiset(const Multiset<key, compare, Alloc>&x) : t(x.t) {}
	Multiset<key, compare, Alloc>& operator = (const Multiset<key, compare, Alloc>&x) {
		t = x.t;
		return *this;
	}

#pragma endregion

#pragma region

	key_compare key_comp() 					const { return t.key_comp(); }
	value_compare value_comp()  			const { return t.key_comp(); }
	iterator begin()    						  { return t.begin(); }
	iterator end()  							  { return t.end(); }
	bool empty()  							const { return t.empty(); }
	size_type size()  						const { return t.size(); }
	size_type max_size()  					const { return t.max_size(); }
	iterator insert(const value_type& x)   		  { return t.insert_equal(x); }
	iterator find(const key_type& x)   			  { return t.find(x); }

#pragma endregion

};























