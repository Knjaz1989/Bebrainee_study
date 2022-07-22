#include "prototype.h"


hash_item *create_hash_item(char* key, char* value) {
    hash_item *item = (hash_item*) malloc (sizeof(hash_item));
    item->key = (char*) malloc (strlen(key) + 1);
    item->value = (char*) malloc (strlen(value) + 1);
    item->next = NULL;
    strcpy(item->key, key);
    strcpy(item->value, value);
    return item;
}

void _delete_hash_item(hash_item *item) {
    free(item->key);
    free(item->value);
    free(item);
}

HashTable* create_table(int size) {
    // Creates a new HashTable
    HashTable *table = (HashTable*)malloc(sizeof(HashTable));
    table->size = size;
    table->full_items = 0;
    table->items = (hash_item **)calloc(table->size, sizeof(hash_item *));
    for (int i=0; i<table->size; i++)
        table->items[i] = NULL;
    return table;
}

void delete_table(HashTable *table) {
    for (int i=0; i<table->size; i++) {
        hash_item *item = table->items[i];
        while (item) {
            hash_item *next = item->next;
            _delete_hash_item(item);
            item = next;
        }

    }
    free(table->items);
    free(table);
}

unsigned long hash_function(char *str, int table_size) {
    unsigned long i = 0;
    for (int j=0; str[j]; j++)
        i += str[j];
    return i % table_size;
}

void insert(HashTable *table, char* key, char* value) {
    int index = hash_function(key, table->size);
    _item_insert(table, index, key, value);
}

void _item_insert(HashTable *table, int index, char* key, char* value) {
    hash_item *item = create_hash_item(key, value);
    if (table->items[index] == NULL) {
        table->items[index] = item;
        table->full_items++;
        return;
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

char *get_value(HashTable *table, char* key) {
    int index = hash_function(key, table->size);
    hash_item *item = table->items[index];
    while (item) {
        if (strcmp(item->key, key) == 0) {
            return strcat(item->value, "\n");
        }
        item = item->next;
    }
    return "There is no such key\n";
}

void delete(HashTable *table, char* key){
    int index = hash_function(key, table->size);
    hash_item *item = table->items[index];
    hash_item *prev_item;
    int count = 0;
    while (item) {
        if (strcmp(item->key, key) == 0) {
            if (count == 0) {
                table->items[index] = item->next;
            } else {
                prev_item->next = item->next;
            }
            _delete_hash_item(item);
            if (table->items[index] == NULL) {
                table->full_items--;
            }
            printf("Success. Key '%s' was deleted\n", key);
            return;
        }
        prev_item = item;
        item = item->next;
        count++;
    }
    printf("There is no such key\n");
}
