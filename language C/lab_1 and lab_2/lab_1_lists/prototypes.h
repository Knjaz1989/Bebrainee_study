//
// Created by igor on 25.04.2022.
//

#ifndef UNTITLED2_STRUCTURES_H
#define UNTITLED2_STRUCTURES_H

#include <stdio.h>
#include "string.h"
#include "stdlib.h"


typedef struct Node {
    char *data;
    struct Node *next;
    struct Node *prev;
} Node;


typedef struct DB {
    int size;
    Node *head;
    Node *tail;
} DB;


DB *create_db();
Node *_create_node(char *value);
char *get_current_node_value(DB *db, int index);
void show_index(DB* db, char *value);
void delete_current_node(DB *db, int index);
void add_node(DB *db, int index, char *value);
void print_db(DB *db);
void add_a_certain_amount_into_db(DB *db, int amount);
int get_db_size(DB *DB);
char *random_value();

#endif //UNTITLED2_STRUCTURES_H
