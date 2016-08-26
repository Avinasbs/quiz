/* Find longest compound word which is a concatenation of other words in the list */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <signal.h>
#include "trie.h"

#define NUM_WORDS_OF_UNIQUE_LEN 100
char word_file[100] = "word.list";

uint8_t word_len_arr[NUM_WORDS_OF_UNIQUE_LEN]; 
uint32_t max_word_len = 0;

/* Algorithm to check if the "word" is a concatenation of other sub words also in the list 
 * Trigger trie_search which will walk the trie from root and return if a leaf is encountered with the leaf index.
 * At this point, trigger another trie_search from this index to the rest of the word.
 * Do this in a loop until the entire word is parsed. */

char *walk_and_check_compound_in_trie (struct trie_node_t *trie_root, char *word)
{
    uint32_t i;
    int idx;

    printf("word: %s\n", word);

    i = 0;
    while (i < strlen(word)) {
	idx = trie_search(trie_root, (word+i));
	if (idx < 0) {
		return NULL;
	} else {
		i = i+ idx+1;
	}
    }

    printf("Compound word: %s\n", word);
    return word;
}

/* Pick the word of length <word_len> and run the walk_and_check_compound_in_trie algorithm on it */
char *parse_walk_and_find(struct trie_node_t *trie_root, uint32_t word_len)
{
    FILE * fp;
    size_t len;
    ssize_t read_len;
    char *word, *walk_result;

    fp = fopen(word_file, "r");
    
    if (fp == NULL)
        return NULL;
  
    while((read_len = getline(&word, &len, fp)) != -1) {
	*(word+strlen(word)-1) = '\0';
	if (strlen(word) == word_len) {
	    walk_result = walk_and_check_compound_in_trie(trie_root, word);
	    if (walk_result) {
		fclose(fp);
	    	return walk_result;
	    }
	}
    }
    fclose(fp);
    return NULL;
}

char *find_longest_compound_word() 
{
    char *word, *parse_result;
    struct trie_node_t *trie_root = trie_node_create(); 
    FILE * fp;
    size_t len;
    ssize_t read_len;

    fp = fopen(word_file, "r");
    if (fp == NULL)
        return NULL;

    /* Read all the words and insert into trie 		*
     * Also, keep info on length of all words present. 	*
     * Also, store the length of longest word 		*/

    while ((read_len = getline(&word, &len, fp)) != -1) {
	*(word+strlen(word)-1) = '\0';
	trie_insert(trie_root, word);
	word_len_arr[strlen(word)] = 1;
      	if (strlen(word) > max_word_len) {
	    max_word_len = strlen(word);
     	}
    }

    /* Parse the list and for all the unique word lengths that exist starting from longest word length. 
     * check if that word has subwords in the list */
    printf("");
    for (uint32_t i = 0; i < max_word_len; i++) {
        printf("");
	if (word_len_arr[max_word_len - i] == 1) {
	    parse_result = parse_walk_and_find(trie_root, max_word_len - i);
	    if (parse_result) {
    		fclose(fp);
	    	return parse_result;
	    }
            printf("");
	}
    }
    fclose(fp);
    return NULL;
}

int main()
{
    printf("Enter the input file name:");
    scanf("%s", word_file);
    find_longest_compound_word();
    return 0;
}
