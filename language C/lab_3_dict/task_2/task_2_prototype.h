//
// Created by igor on 09.05.2022.
//

#ifndef TASK_2_TASK_2_PROTOTYPE_H
#define TASK_2_TASK_2_PROTOTYPE_H

void _insert_bus(HashTable *Table, int index, char *key);
void insert_buses(HashTable *Table);
void write_buses_to_file(HashTable *Table, char *name_file);
int count_buses_from_file(char *name_file);

#endif //TASK_2_TASK_2_PROTOTYPE_H
