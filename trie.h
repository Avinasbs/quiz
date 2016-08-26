typedef unsigned char uint8_t;

#define NUM_TRIE_CHILDREN (26)

typedef struct trie_node_t {
    struct trie_node_t *children[NUM_TRIE_CHILDREN];
    uint8_t     leaf;
    uint32_t    ref_cnt;
} trie_node_t;

struct trie_node_t *trie_node_create ();
struct trie_node_t *
trie_get_child(struct trie_node_t *node, char child);
void
trie_set_child(struct trie_node_t *node, struct trie_node_t *new_node, char child);
void
trie_insert (struct trie_node_t *node, const char *word);
int
trie_search (struct trie_node_t *node, const char *word);
