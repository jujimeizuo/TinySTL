#ifndef _RB_TREE_ITERATOR_
#define _RB_TREE_ITERATOR_

#include "RBTreeNode.h"
#include <memory>

struct rb_tree_base_iterator {
	typedef __rb_tree_node_base::base_ptr 		base_ptr;
	typedef std::bidirectional_iterator_tag     iterator_category;
	typedef ptrdiff_t 							difference_type;
	base_ptr  									node;

	void increment() {
		if(node -> right != 0) {
			node = node -> right;
			while(node -> left != 0) {
				node = node -> left;
			}
		} else {
			base_ptr y = node -> parent;
			while(node == y -> right) {
				node = y;
				y = y -> parent;
			}
			if(node -> right != y) {
				node = y;
			}
		}
	}

	void decrement() {
		if(node -> color == __rb_tree_red && node -> parent -> parent == node) {
			node = node -> right;
		} else if(node -> left != 0) {
			base_ptr y = node -> left;
			while(y -> right != 0) {
				y = y -> right;
			}
			node = y;
		} else {
			base_ptr y = node -> parent;
			while(node == y -> left) {
				node = y;
				y = y -> parent;
			}
			node = y;
		}
	}
};

template<class value, class ref, class ptr>
struct __rb_tree_iterator : public rb_tree_base_iterator {

#pragma region

	typedef value 										value_type;
	typedef ref  										reference;
	typedef ptr 										pointer;
	typedef __rb_tree_iterator<value, value&, value*>  	iterator;
	typedef __rb_tree_iterator<value, ref, ptr>  		self;
	typedef __rb_tree_node<value>*   					link_type;
	typedef size_t   									size_type;

#pragma endregion

#pragma region

	__rb_tree_iterator() {}
	__rb_tree_iterator(link_type x) { node = x; }
	__rb_tree_iterator(const iterator& it) { node = it.node; }

#pragma endregion

#pragma region

	reference operator*() const { return link_type(node) -> value_field; }
	pointer operator->()  const { return *(operator*()); }
	__rb_tree_color_type color(){ return node -> color == __rb_tree_red ? 0 : 1; }
	self& operator++() 	  const { increment(); return *this; }
	self& operator--()    const { decrement(); return *this; }
	self& operator++(int) {
		self tmp = *this;
		increment();
		return tmp;
	}
	self& operator--(int) {
		self tmp = *this;
		decrement();
		return tmp;
	}
	bool operator==(const self& x) const { return x.node == node; }
	bool operator!=(const self& x) const { return x.node != node; }
	
#pragma endregion

};

#endif