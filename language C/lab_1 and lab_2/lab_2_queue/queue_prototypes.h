//
// Created by igor on 10.05.2022.
//

#ifndef LAB_2_QUEUE_QUEUE_PROTOTYPES_H
#define LAB_2_QUEUE_QUEUE_PROTOTYPES_H

void push(DB *db, char *value);
char* pop(DB *db);
char *pop_random_value(DB *db);
void insert_into_random_place(DB *db, int amount);
char *_get_value_and_delete_node(DB *db, int index);

#endif //LAB_2_QUEUE_QUEUE_PROTOTYPES_H
