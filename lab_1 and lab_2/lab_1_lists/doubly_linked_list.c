#include "prototypes.h"



int main() {
    DB *db = create_db();
    add_node(db, 8, "Hello");
    add_node(db, 0, "World!");
    add_node(db, -1,"Everyone");
    print_db(db);
    printf("[%s]\n", get_current_node_value(db, 2));
    show_index(db, "Hello");
    delete_current_node(db, 0);
    add_a_certain_amount_into_db(db, 1000000);
    print_db(db);
    printf("%d\n", get_db_size(db));

    return 0;
}
