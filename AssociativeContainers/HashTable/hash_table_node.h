#ifndef _HASH_TABLE_NODE_
#define _HASH_TABLE_NODE_

template<class value>
struct hash_table_node {
	hash_table_node* next;
	value val;
};

#endif