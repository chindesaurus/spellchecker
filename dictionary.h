/*
 * @(#) dictionary.h
 *
 * Declares a dictionary's functionality.
 *
 * @author chindesaurus
 * @version 1.00
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

/**
 * Returns true if word is in dictionary else false.
 */
extern bool check(const char* word);

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
extern bool load(const char* dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
extern unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
extern bool unload(void);


#endif // DICTIONARY_H
