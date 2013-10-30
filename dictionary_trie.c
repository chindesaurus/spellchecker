/*
 * @(#) dictionary_trie.c
 *
 * Implements a dictionary's functionality using a trie.
 *
 * @author chindesaurus
 * @version 1.00
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"


/* letters in the alphabet plus apostrophe */
#define CHARS 27

/* number of words loaded in dictionary */
static unsigned int SIZE = 0;


/* defines a node of the trie */
typedef struct node {
    // does this node represent the last letter of a word?
    bool is_word;

    // each element points to another node
    struct node *children[CHARS];
} node;

node *root = NULL;  // root of the trie


/* static function prototypes */
static void toLowerCase(char *word); 
static void freeNode(node *curr);


/**
 * Returns true if word is in dictionary else false.
 */
bool 
check(const char* word) {
    
    // copy word to key
    char key[strlen(word) + 1];
    strcpy(key, word);
    
    // check is case-insensitive
    toLowerCase(key);

    node *iterator = root;

    // for each letter in the word
    for (int i = 0; i < strlen(key); i++) {
     
        int index = key[i] - 'a';
        if (key[i] == '\'')
            index = CHARS - 1;
        
        // if we haven't reached the end of a word,
        // continue down the trie
        if (iterator->children[index] != NULL)
            iterator = iterator->children[index];
        else
            return false;
    }

    // at the end of the word, check if the node
    // marks the end of a dictionary word
    return iterator->is_word;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool 
load(const char* dictionary) {
    
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
        return false;

    /* initialize the trie */
    root = malloc(sizeof(*root));
    if (root == NULL) {
        fclose(dict); 
        return false;
    }  
    root->is_word = false;
    for (int i = 0; i < CHARS; i++)
        root->children[i] = NULL;

    // for iterating through the trie
    node *iterator = root;

    char input;
    while ( (input = fgetc(dict)) != EOF) {
        
        // ensure every letter is lower case
        input = tolower(input);

        int index = input - 'a';
        if (input == '\'')
            index = CHARS - 1;
            
        // end of word 
        if (input == '\n') {
            iterator->is_word = true;
            SIZE += 1; 

            // reset iterator
            iterator = root;
        }

        else { 
            if (iterator->children[index] == NULL) {
                
                // if NULL, allocate space for new node, initialize it,
                // and have children[index] point to it
                node *new = malloc(sizeof(*new));
                if (new == NULL) {
                    fclose(dict); 
                    return false;
                }

                for (int i = 0; i < CHARS; i++)
                    new->children[i] = NULL;
                new->is_word = false;

                iterator->children[index] = new;
            }
            // move iterator to new node
            iterator = iterator->children[index];
       }
    }
    
    fclose(dict);
   
    // w00t 
    return true;
}


/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int 
size(void) {
    return SIZE;
}


/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool 
unload(void) {
    freeNode(root);
    free(root);
    return true;
}


/**
 * Frees all nodes above curr.
 */
void
freeNode(node *curr) {

    if (curr == NULL)
        return;

    // iterate through all of curr's children
    for (int i = 0; i < CHARS; i++) {
        // for each non-NULL child, recursively free it
        if (curr->children[i] != NULL)
            freeNode(curr->children[i]);
        
        // if node has no children, free it
        free(curr->children[i]);
    }
}


/**
 * Converts every letter of word to lower case.
 */
static void
toLowerCase(char *word) {
    for (int i = 0; word[i]; i++)
        word[i] = tolower(word[i]);
}
