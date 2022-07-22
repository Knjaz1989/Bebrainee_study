#include "prototypes.h"


void push(DB *db, char *value) {
    int index = db->size;
    add_node(db, index, value);
}
char *_get_value_and_delete_node(DB *db, int index) {
    char *value = get_current_node_value(db, index);
    delete_current_node(db, index);
    return value;
}

char* pop(DB *db) {
    return _get_value_and_delete_node(db, 0);
}

void insert_into_random_place(DB *db, int amount) {
    for (int i = 0; i < amount; i++) {
        int index = rand() % db->size;
        char *value = random_value();
        add_node(db, index, value);
    }
}

char *pop_random_value(DB *db) {
    int index = rand() % db->size;
    return _get_value_and_delete_node(db, index);
}
