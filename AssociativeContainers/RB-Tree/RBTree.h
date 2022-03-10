#ifndef _RB_TREE_
#define _RB_TREE_

#include "../../allocator/globalConstruct.h"
#include "../../allocator/alloc.h"
#include "RBTreeNode.h"
#include "RBTreeIterator.h"

template<class key, class value, class keyOfValue, class compare, class Alloc = Allocator<key>>
class RBTree {

#pragma region

protected :
	typedef void*  										void_pointer;
	typedef __rb_tree_node_base*  						base_ptr;
	typedef __rb_tree_node<value>   					rb_tree_node;
	typedef simple_alloc<rb_tree_node, Alloc>  			rb_tree_node_allocator;
	typedef __rb_tree_color_type    					color_type;

public :
	typedef key   										key_type;
	typedef value  	 									value_type;
	typedef value_type*   								pointer;
	typedef const value*   								const_pointer;
	typedef value_type&  								reference;
	typedef const value_type&  							const_reference;
	typedef rb_tree_node*   							link_type;
	typedef size_t   									size_type;
	typedef ptrdiff_t   								difference_type;

#pragma endregion

#pragma region

protected :
	size_type node_count;
	link_type header;
	compare key_compare;

public :
	typedef __rb_tree_iterator<value_type, reference, pointer> iterator;

#pragma endregion

#pragma region

protected :
	link_type get_node() 			{ return rb_tree_node_allocator::allocate(); }
	
	void put_node(link_type p) 		{ rb_tree_node_allocator::deallocate(p); }
	
	link_type create_node(const value_type& x) {
		link_type tmp = get_node();
		construct(&tmp -> value_field, x);
		return tmp;
	}
	
	link_type clone_node(link_type x) {
		link_type tmp = cteate_node(x -> value_field);
		tmp -> color = x -> color;
		tmp -> left = 0;
		tmp -> right = 0;
		return tmp;
	}

	void destroy_node(link_type p) {
		destroy(&p -> value_field);
		put_node(p);
	}

private :
	void init() {
		header = get_node();
		color(header) = __rb_tree_red;
		root() = 0;
		leftmost() = header;
		rightmost() = header;
	}

public :
	RBTree(const compare& cmp = compare()) : node_count(0), key_compare(cmp) { init(); }
	~RBTree() { put_node(header); }

#pragma endregion

#pragma region

protected :
	link_type& root() const 				{ return (link_type&) header -> parent; }
	
	link_type& leftmost() const 			{ return (link_type&) header -> left; }

	link_type& rightmost() const 			{ return (link_type&) header -> right; }

	static link_type& left(link_type x) 	{ return (link_type&) (x -> left); }

	static link_type& right(link_type x) 	{ return (link_type&) (x -> right); }

	static link_type& parent(link_type x)	{ return (link_type&) (x -> parent); }

	static reference value(link_type x) 	{ return (x -> value_field); }

	static color_type& color(link_type x) 	{ return (color_type) (x -> color); }

	static const key& key(base_ptr x) 		{ return keyOfValue() (value(link_type(x))); }

	static link_type minimum(link_type x) 	{ return (link_type) __rb_tree_node_base::minimum(x); }

	static link_type maximum(link_type x) 	{ return (link_type) __rb_tree_node_base::maximum(x); }

#pragma endregion

#pragma region

public :
	value_type root_value() 		{ return value((link_type) header -> parent); }

	compare key_comp() const 		{ return key_compare; }

	iterator begin()  				{ return leftmost(); }

	iterator end() 					{ return header; }

	bool empty() const 				{ return node_count == 0; }

	size_type size() const 			{ return node_count; }

	size_type max_size() const 		{ return size_type(-1); }

#pragma endregion

#pragma region

public :
	std::pair<iterator, bool> insert_unique(const value_type& v) {
		link_type y = header;
		link_type x = root();
		bool comp = true;
		while(x != 0) {
			y = x;
			comp = key_compare(keyOfValue()(v), key(x));
			x = comp ? left(x) : right(x);
		}
		iterator j = iterator(y);
		if(comp) {
			if(begin() == j) {
				return std::pair<iterator, bool>(__insert(x, y, v), true);
			}
			else {
				j--;
			}
		}
		if(key_compare(key(j.node), keyOfValue()(v))) {
			return std::pair<iterator, bool>(__insert(x, y, v), true);
		}
		return std::pair<iterator, bool>(j, false);
	}

	iterator insert_equal(const value_type& v) {
		link_type y = header;
		link_type x = root();
		while(x != 0) {
			y = x;
			x = key_compare(keyOfValue()(v), key(x)) ? left(x) : right(x);
		}
		return __insert(x, y, v);
	}

	iterator find(const value_type& k) {
		link_type y = header;
		link_type x = root();
		while(x != 0) {
			if(!key_compare(key(x), k)) {
				y = x;
				x = left(x);
			} else {
				x = right(x);
			}
		}
		iterator j = iterator(y);
		return (j == end() | key_compare(k, key(j.node))) ? end() : j;
	}

private :
	iterator __insert(base_ptr x_, base_ptr y_, const value_type& v) {
		link_type x = (link_type) x_;
		link_type y = (link_type) y_;
		link_type z;

		if(y == header || x != 0 || key_compare(keyOfValue()(v), key(y))) {
			z = create_node(v);
			left(y) = z;
			if(y == header) {
				root() = z;
				rightmost() = z;
			} else if(y == leftmost()) {
				leftmost() = z;
			}
		} else {
			z = create_node(v);
			right(y) = z;
			if(y == rightmost()) {
				rightmost() = z;
			}
		}
		parent(z) = y;
		left(z) = 0;
		right(z) = 0;
		__rb_tree_rebalance(z, header -> parent);
		++node_count;
		return iterator(z);
	}

	inline void __rb_tree_rebalance(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		x -> color = __rb_tree_red;
		while(x != root && x -> parent -> color == __rb_tree_red) {
			if(x -> parent == x -> parent -> parent -> left) {
				__rb_tree_node_base* y = x -> parent -> parent -> right;
				if(y && y -> color == __rb_tree_red) {
					x -> parent -> color = __rb_tree_black;
					y -> color = __rb_tree_black;
					x -> parent -> parent -> color = __rb_tree_red;
					x = x -> parent -> parent;
				} else {
					if(x == x -> parent -> right) {
						x = x -> parent;
						__rb_tree_rotate_left(x, root);
					}
					x -> parent -> color = __rb_tree_black;
					x -> parent -> parent -> color = __rb_tree_red;
					__rb_tree_rotate_right(x -> parent -> parent, root);
				}
			} else {
				__rb_tree_node_base* y = x -> parent -> parent -> left;
				if(y && y -> color == __rb_tree_red) {
					x -> parent -> color = __rb_tree_black;
					y -> color = __rb_tree_black;
					x -> parent -> parent -> color = __rb_tree_red;
					x = x -> parent -> parent;
				} else {
					if(x == x -> parent -> left) {
						x = x -> parent;
						__rb_tree_rotate_right(x, root);
					}
					x -> parent -> color = __rb_tree_black;
					x -> parent -> parent -> color = __rb_tree_red;
					__rb_tree_rotate_left(x -> parent -> parent, root);
				}
			}
		}
		root -> color = __rb_tree_black;
	}

	inline void __rb_tree_rotate_left(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x -> right;
		x -> right = y -> left;
		if(y -> left != 0) {
			y -> left -> parent = x;
		}
		y -> parent = x -> parent;
		if(x == root) {
			root = y;
		} else if(x == x -> parent -> left) {
			x -> parent -> left = y;
		} else {
			x -> parent -> right = y;
		}
		y -> left = x;
		x -> parent = y;
	}

	inline void __rb_tree_rotate_right(__rb_tree_node_base* x, __rb_tree_node_base*& root) {
		__rb_tree_node_base* y = x -> left;
		x -> left = y -> right;
		if(y -> right != 0) {
			y -> right -> parent = x;
		}
		y -> parent = x -> parent;
		if(x == root) {
			root = y;
		} else if(x == x -> parent -> right) {
			x -> parent -> right = y;
		} else {
			x -> parent -> left = y;
		}
		y -> right = x;
		x -> parent = y;
	}

#pragma endregion



};

#endif
























