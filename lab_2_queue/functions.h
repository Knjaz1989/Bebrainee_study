//
// Created by igor on 25.04.2022.
//
#include <stdlib.h>
#include <string.h>
#ifndef UNTITLED2_FUNCTIONS_H
#define UNTITLED2_FUNCTIONS_H


DB *create_db() {
    DB *tmp_db = (DB *)malloc(sizeof(DB));
    tmp_db->size = 0;
    tmp_db->head = tmp_db->tail = NULL;
    return tmp_db;
}


Node *create_node(char *value) {
    Node *tmp_node = (Node *)malloc(sizeof(Node));
    tmp_node->data = (char *)malloc(sizeof(char) * strlen(value) + 1);
    tmp_node->data = strcpy(tmp_node->data, value);
    tmp_node->next = NULL;
    return tmp_node;
}


void add_node(DB *db, char *value) {
    Node *new_node = create_node(value);
    db->size++;

    if (db->head == NULL & db->tail == NULL) {
        db->head = db->tail = new_node;
        return;
    }
    Node *tmp = db->tail;
    tmp->next = new_node;
    db->tail = new_node;
}


void get_node(DB *db) {
    if (db->size == 0) {
        printf("The database is empty\n");
        return;
    }
    Node *tmp = db->head;
    if (db->size == 1) {
        db->head = db->tail = NULL;
    } else {
        db->head = tmp->next;
    }
    db->size--;
    printf("[%s]\n", tmp->data);
    free(tmp);
}


void print_db(DB *db) {
    if (db->head == NULL) {
        printf("The database is empty\n");
        return;
    }
    Node *tmp = db->head;
    while (tmp) {
        printf("[%s]\n", (*tmp).data);
        tmp = tmp->next;
    }
}


void add_a_certain_amount_into_db(DB *db, int amount) {

    for (int i = 0; i < amount; i++) {
        Node *new_node = create_node("temporary value");
        if (db->head == NULL & db->tail == NULL) {
            db->head = db->tail = new_node;
            continue;
        }
        Node *last_node = db->tail;
        last_node->next = new_node;
        db->tail = new_node;
        db->size++;
    }
}


#endif //UNTITLED2_FUNCTIONS_H
