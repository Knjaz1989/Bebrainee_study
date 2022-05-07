//
// Created by igor on 05.05.2022.
//

#ifndef _C_DICT_PROTOTYPE_H
#define _C_DICT_PROTOTYPE_H

//-----------------------Structures------------------------------

// Define the HashTable item here
typedef struct hash_item {
    char *key;
    char *value;
    struct hash_item *next;
} hash_item;

// Define the HashTable
typedef struct HashTable {
    // Contains an array of pointers to items
    hash_item** items;
    int size;
} HashTable;

//-----------------------Prototypes------------------------------

hash_item *create_hash_item(char* key, char* value);
void _delete_hash_item(hash_item *item);
HashTable *create_table();
void delete_table(HashTable* table);
void insert(HashTable* table, char *key, char *value);
char *get_value(HashTable* table, char* key);
void delete(HashTable* table, char* key);
unsigned long hash_function(char *str);



#endif //_C_DICT_PROTOTYPE_H
