#include "prototype.h"
#include "task_1_prototype.h"
#include "task_2_prototype.h"


void _insert_bus(HashTable *Table, int index, char *key) {
    _item_insert(Table, index, key, "");
}

void insert_buses(HashTable *Table) {
    for (int i = 1; i < Table->size; i++) {
        int buses_amount = rand() % 20 + 5;
        for (int j = 0; j < buses_amount; j++) {
            char *auto_number = _random_auto_number();
            _insert_bus(Table, i, auto_number);
        }
    }
}

void write_buses_to_file(HashTable *Table, char *name_file) {
    FILE *file = fopen(name_file, "w");
    if (file == NULL) {
        printf("file can't be opened \n");
        return;
    }
    for (int i = 1; i < Table->size; i++) {
        hash_item *tmp = Table->items[i];
        while (tmp) {
            if (i == Table->size -1 && tmp->next == NULL) {
                fprintf(file, "%d", i);
                break;
            }
            fprintf(file, "%d, ", i);
            tmp = tmp->next;
        }
    }
    fclose(file);
}

int count_buses_from_file(char *name_file) {
    int count = 0;
    FILE *file = fopen(name_file, "r");
    if (file == NULL) {
        printf("file can't be opened \n");
        exit(1);
    }
    while (1) {
        int letter_num = fgetc(file);
        if (letter_num == -1) {
            break;
        }
        if (letter_num == 44) {
            count++;
        }
    }
    fclose(file);
    return count + 1;
}
