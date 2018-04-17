// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>

#include "dictionary.h"

// Globally define a struct, a new data type, called 'node' which will hold a word
// and a pointer - this will be used to implement a linked list
typedef struct node {
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Globally define an integer to count the number of words in the dictionary
// Initially, it will be 0
int word_count = 0;

// Implement a hashtable
#define HASHTABLE 27
node* hashtable[HASHTABLE];

// Hash function is from CS50 Study and found at following reddit thread:
// https://www.reddit.com/r/cs50/comments/24b3sj/hash_function_httpsstudycs50nethashtables_question/

int hash(const char* word)
{
    // initialize index to 0
    int index = 0;

    // sum ascii values
    for (int i = 0; word[i] != '\0'; i++)
        // search for lower cases words
        index += tolower(word[i]);

    // mod by size to stay w/in bound of table
    return index % HASHTABLE;
}

// Globally define a boolean for tracking the load and unload functions
// Initially, it will be false, as nothing is loaded yet
bool load_bool = false;

// 'Check' determines whether or not a word is in the dictionary
// that has been loaded to memory (aka the hash table)
// It will return true if word is in dictionary, else false
bool check(const char *word)
{
    // If the word exists, it can be found in the hash table
    // The word will be within the linked list at location hastable[hash(word)]
    node* cursor = hashtable[hash(word)];

    // Traverse the linked list to find the whether the word exists in memory
    while (cursor != NULL) {

        // use strcasecmp to see if the value at that location is equal to the word
        if (strcasecmp(cursor->word, word) == 0)
            return true;

        cursor = cursor->next;
    }

    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    // Open the dictionary in read-mode ("r")
    // If unable to open the dictionary, return false
    FILE* file = fopen(dictionary, "r");
    if (file == NULL) {
        return false;
    }

    // create an array for word to be stored in
    char word[LENGTH+1];

    // Iterate over each line of the dictionary/file until the
    // end of file (EOF) is reached - loading each word to memory
    while (fscanf(file, "%s", word) != EOF)
    {
        // Increment word_count for each new word
        word_count++;

        // Alloacate space in memory for the new word
        node* new_word = malloc(sizeof(node));

        // Copy word into the new_word
        strcpy(new_word->word, word);

        // Hash the new_word to determine the index of the array
        // where the word will be stored
        int hash_index = hash(word);

        // If the new node is pointing to NULL then there
        // is nothing at that array location yet. Therefore,
        // add the new_word to that array loaction and
        // make its pointer point to NULL
        if (hashtable[hash_index] == NULL)
        {
            hashtable[hash_index] = new_word;
            new_word->next = NULL;
        }

        // If the hashtable array is not empty at thta index, append
        // the linked list at that array location
        else
        {
            new_word->next = hashtable[hash_index];
            hashtable[hash_index] = new_word;
        }

    }

    // Close the dictionary file when iterations are complete
    fclose(file);

    // Change the global bool to indicate that the dictionary was successfully loaded
    load_bool = true;

    // Return true when dictionary is successfully loaded to memory
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // If load_bool from the load function is true, return the word_count
    // which summed the number of words in the dictionary
    if (load_bool == true) {
        return word_count;
    }
    // If load_bool from the load function is false, return 0
    else {
        return 0;
    }

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < HASHTABLE; i++) {

        // Iterate over each location in the hashtable
        // In some cases, the first location will be the head of a linked list
        node *cursor = hashtable[i];

        // Starting with the head of the linked list, create a temporary
        // node cursor, iterate over each element of the list, freeing the memory
        // once you have advanced to the cursor to the next element
        while (cursor != NULL) {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }

    }

    load_bool = false;

    return true;
}
