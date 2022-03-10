#ifndef _RB_TREE_NODE_
#define _RB_TREE_NODE_

typedef bool __rb_tree_color_type;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {
	typedef __rb_tree_color_type color_type;
	typedef __rb_tree_node_base* base_ptr;

	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minimum(base_ptr x) {
		while(x -> left != 0) {
			x = x -> left;
		}
		return x;
	}

	static base_ptr maximun(base_ptr x) {
		while(x -> right != 0) {
			x = x -> right;
		}
		return x;
	}

	template<class value>
	struct __rb_tree_node : public __rb_tree_node_base {
		typedef __rb_tree_node<value>* link_type;
		value value_field;
	};
};

#endif