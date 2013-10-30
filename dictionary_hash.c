/*
 * @(#) dictionary_hash.c
 *
 * Implements a dictionary's functionality 
 * using a hash table.
 *
 * @author chindesaurus
 * @version 1.00
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dictionary.h"

/* array size of hash table */
#define BUCKET_COUNT 65521

/* number of words loaded in dictionary */
static unsigned int SIZE = 0;

/* defines a node of the table */
typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
} node;

static node *table[BUCKET_COUNT] = { NULL };


/* static function prototypes */
static int hash(char *word);
static void toLowerCase(char *word); 


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

    // hash the key and go to that place in the table
    node *ptr = table[hash(key)];

    while (ptr != NULL) {
        // have we found the word?
        if (strcmp(ptr->word, key) == 0)
            return true;
        // keep looking...
        else
            ptr = ptr->next;
    }
    
    // didn't find word in the dictionary
    return false;
}


/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool 
load(const char* dictionary) {
    
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
        return false;

    while (!feof(dict)) {

        /* create a new node */
        // allocate space for new node
        node *new = malloc(sizeof(*new));
        if (new == NULL) {
            fclose(dict); 
            return false;
        }  
 
        // read a word from the dictionary    
        int check = fscanf(dict, "%s", new->word);
        
        // keep track of how many words are loaded in dictionary
        if (check == 1)
            SIZE += 1;

 
        /* put the new node in the hash table */
        int index = hash(new->word);
    
        // linked list at this index doesn't exist yet
        if (table[index] == NULL) {
            table[index] = new;
            new->next = NULL;
        }
        // linked list already exists, so add to the beginning of it
        else {
            new->next = table[index];
            table[index] = new;
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
   
    // go through the entire hash table 
    for (int i = 0; i < BUCKET_COUNT; i++) {
        // whereever there is a nonempty bucket, free the entire linked list
        if (table[i] != NULL) {
            node *ptr = table[i];
            while (ptr != NULL) {
                node *prev = ptr;
                ptr = ptr->next;
                free(prev);
            }
        }
    }
    return true;
}


/**
 * Copypasta from COS 217.
 * Returns a hash code in [0, BUCKET_COUNT - 1] for word.
 */
static int
hash(char *word) {
    assert(word != NULL);
    
    enum {HASH_MULTIPLIER = 65599};
    int i;
    unsigned int uiHash = 0U;
    
    for (i = 0; word[i] != '\0'; i++)
        uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)word[i];
   
    return (int)(uiHash % (unsigned int)BUCKET_COUNT);
}


/**
 * Converts every letter of word to lower case.
 */
static void
toLowerCase(char *word) {
    for (int i = 0; word[i]; i++)
        word[i] = tolower(word[i]);
}
