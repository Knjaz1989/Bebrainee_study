//
// Created by igor on 25.04.2022.
//

#ifndef UNTITLED2_STRUCTURES_H
#define UNTITLED2_STRUCTURES_H


typedef struct Node {
    char *data;
    struct Node *next;
} Node;


typedef struct DB {
    int size;
    Node *head;
    Node *tail;
} DB;


#endif //UNTITLED2_STRUCTURES_H
