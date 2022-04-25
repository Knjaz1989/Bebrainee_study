#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structures.h"
#include "functions.h"



int main() {
    DB *db = create_db();
    add_node_to_end(db, "Hello");
    add_node_to_end(db, "World!");
    add_node_to_begin(db, "Everyone");
    print_db(db);
    printf("[%s]\n", get_current_node_value(db, 2));
    show_index(db, "Hello");
    delete_current_node(db, 0);
    put_node_into_a_current_place(db, 1, "all The");
    put_node_into_a_current_place(db, 0, "Everyone");
    put_node_into_a_current_place(db, 3, "--");
    add_a_certain_amount_into_db(db, 10000);
    print_db(db);
    return 0;
}
