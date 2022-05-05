#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 50000 // Size of the Hash Table


// Define the HashTable item here
typedef struct hash_item {
    char *key;
    char *value;
    struct hash_item *next;
} hash_item;

typedef struct HashTable {
    // Contains an array of pointers to items
    hash_item** items;
    int size;
} HashTable;

hash_item *create_hash_item(char* key, char* value);
void delete_hash_item(hash_item *item);
HashTable *create_table(int size);
void delete_table(HashTable* table);
void insert(HashTable* table, char *key, char *value);
unsigned long hash_function(char *str);



int main() {
    HashTable *Table = create_table(1000);
    insert(Table, "good", "tomato");
    delete_table(Table);
    return 0;
}



hash_item *create_hash_item(char* key, char* value) {
    hash_item *item = (hash_item*) malloc (sizeof(hash_item));
    item->key = (char*) malloc (strlen(key) + 1);
    item->value = (char*) malloc (strlen(value) + 1);
    item->next = NULL;
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

void delete_hash_item(hash_item *item) {
    free(item->key);
    free(item->value);
    free(item->next);
    free(item);
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->items = (hash_item **)calloc(table->size, sizeof(hash_item *));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    return table;
}

void delete_table(HashTable *table) {
    for (int i=0; i<table->size; i++) {
        hash_item *item = table->items[i];
        if (item != NULL)
            delete_hash_item(item);
    }
    free(table->items);
    free(table);
}

unsigned long hash_function(char *str) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % CAPACITY;
}

void insert(HashTable* table, char* key, char* value) {
    hash_item *item = create_hash_item(key, value);
    int index = hash_function(key);

    if (table->items[index] == NULL) {
        table->items[index] = item;
    }
    hash_item *current_item = table->items[index];
    while (current_item) {
        if (strcmp(current_item->key, key) == 0) {
            free(current_item->value);
            current_item->value = (char*) malloc (strlen(value) + 1);
            strcpy(current_item->value, value);
            return;
        }
        if (current_item->next == NULL) {
            current_item->next = item;
            return;
        }
        current_item = current_item->next;
    }
}