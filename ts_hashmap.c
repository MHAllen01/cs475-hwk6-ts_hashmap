#include <limits.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ts_hashmap.h"

/**
 * Creates a new thread-safe hashmap. 
 *
 * @param capacity initial capacity of the hashmap.
 * @return a pointer to a new thread-safe hashmap.
 */
ts_hashmap_t *initmap(int capacity) {
  // TODO

  // New hashmap
  ts_hashmap_t *hashmap = (ts_hashmap_t*) malloc (sizeof(ts_hashmap_t));

  // Table to hold Linked Lists
  ts_entry_t **table = (ts_entry_t**) malloc (sizeof(ts_entry_t*) * capacity);

  // Allocate table
  for (int i = 0; i < capacity; i++) {
    table[i] = NULL;
  }

  // Initialize vars
  hashmap->table = table;
  hashmap->capacity = capacity;
  hashmap->size = 0;

  return hashmap;
}

/**
 * Obtains the value associated with the given key.
 * @param map a pointer to the map
 * @param key a key to search
 * @return the value associated with the given key, or INT_MAX if key not found
 */
int get(ts_hashmap_t *map, int key) {
  // TODO
  // Look through index in the array
  unsigned int index = ((unsigned int) key) % map->capacity;

  // Get the current entry in the map
  ts_entry_t *entry = map->table[index];
  
  // While we're not at the tail and current node isn't null
  while (entry != NULL && entry->next != NULL) {
      // Key found, return value
      if (entry->key == key) {
        return entry->value;
      }

      // Traverse the Linked List
      entry = entry->next;
  }

  // Check last node of Linked List
  if (entry != NULL && entry->key == key) {
    return entry->value;
  }

  // Key wasn't found
  return INT_MAX;
}

/**
 * Associates a value associated with a given key.
 * @param map a pointer to the map
 * @param key a key
 * @param value a value
 * @return old associated value, or INT_MAX if the key was new
 */
int put(ts_hashmap_t *map, int key, int value) {
  // TODO

  // Calculate the index
  unsigned int index = ((unsigned int) key) % map->capacity;

  // Get the current entry in the map
  ts_entry_t *entry = map->table[index];

  // Assign entry if the keys don't match
  // Traverse the Linked List
  while (entry != NULL && entry->key != key) {
    entry = entry->next;
  }

  if (entry == NULL) {
    // Key doesn't exist
    
    // Allocate for new node
    entry = (ts_entry_t*) malloc(sizeof(ts_entry_t));

    // Set the new key and values to the new node
    entry->key = key;
    entry->value = value;
    entry->next = map->table[index];

    // Set the new node in the table
    map->table[index] = entry;
    
    // Increase the size by 1
    map->size = map->size + 1;

    // Return INT_MAX since the key is new
    return INT_MAX;

  } else {
    // Key exists

    // Get the old value of the current key
    int val = entry->value;

    // Set the old value to the passed in value
    entry->value = value;

    // Return the old value
    return val;
  }

  return INT_MAX;
}

/**
 * Removes an entry in the map
 * @param map a pointer to the map
 * @param key a key to search
 * @return the value associated with the given key, or INT_MAX if key not found
 */
int del(ts_hashmap_t *map, int key) {
  // TODO

  // Calculate the index
  unsigned int index = ((unsigned int) key) % map->capacity;

  // Get the current entry in the map
  ts_entry_t *entry = map->table[index];

  // Get the last previous entry
  ts_entry_t *prevEntry = NULL;


  // Assign entry if the keys don't match
  // Traverse the Linked List
  while (entry != NULL && entry->key != key) {
    prevEntry = entry;
    entry = entry->next;
  }

  if (entry == NULL) {
    // Key doesn't exist
    
    return INT_MAX;

  } else {
    // Key exists

    // Get the old value of the current key
    int val = entry->value;

    if (prevEntry == NULL) {
      map->table[index] = entry->next;
    } else {
      prevEntry->next = entry->next;
    }
   
    map->size = map->size - 1;
    // Return the old value
    return val;
  }

  return INT_MAX;
}


/**
 * @return the load factor of the given map
 */
double lf(ts_hashmap_t *map) {
  return (double) map->size / map->capacity;
}

/**
 * Prints the contents of the map
 */
void printmap(ts_hashmap_t *map) {
  for (int i = 0; i < map->capacity; i++) {
    printf("[%d] -> ", i);
    ts_entry_t *entry = map->table[i];
    while (entry != NULL) {
      printf("(%d,%d)", entry->key, entry->value);
      if (entry->next != NULL)
        printf(" -> ");
      entry = entry->next;
    }
    printf("\n");
  }
}