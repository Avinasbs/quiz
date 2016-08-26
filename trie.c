/* trie.c */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include "trie.h"

struct trie_node_t * 
trie_node_create ()
{
    uint8_t i;
    struct trie_node_t *node;
    node = calloc(1, sizeof(trie_node_t));
    // node->ref_cnt = 1;
    return node;
}

struct trie_node_t * 
trie_get_child(struct trie_node_t *node, char child)
{
    assert( (child >= 97) && (child <= 122) );
    return node->children[child - 97];
}

void
trie_set_child(struct trie_node_t *node, struct trie_node_t *new_node, char child)
{
    assert( (child >= 97) && (child <= 122) );
    node->children[child-97] = new_node;
}

/* Insert a word into the trie. For each node/letter created, increment a ref_cnt */
void
trie_insert (struct trie_node_t *node, const char *word)
{
    uint32_t i;
    struct trie_node_t *new_node;
    if (node == NULL) {
	return;
    }
    for (i = 0; i < strlen(word); i++) {
    	if (trie_get_child(node, *(word+i)) == NULL) {
	    new_node = trie_node_create();
    	    trie_set_child(node, new_node, *(word+i));
	    node = trie_get_child(node, *(word+i));
	} else {
	    node = trie_get_child(node, *(word+i));
	    // (node->ref_cnt)++;
	}
    }
    node->leaf = 1;
}

/* Search the trie for the word. return the index of the first leaf encountered. else return -1 */
int
trie_search (struct trie_node_t *node, const char *word)
{
    uint32_t i;
    for (i = 0; i < strlen(word); i++) {
	node = trie_get_child(node, *(word+i));
	if (node == NULL) {
	    return (-1);
	}
	if (node->leaf) {
	    return i;
	}
    }
    return (-1);
}

int trie_test()
{
    struct trie_node_t *trie_root = trie_node_create();
    trie_insert(trie_root, "road");
    trie_insert(trie_root, "roadrunner");
    trie_insert(trie_root, "roadmap");
    trie_insert(trie_root, "runner");
    
    printf("search for %s returned %d\n", "roadrunner", trie_search(trie_root, "roadrunner"));
    return 0;
}
