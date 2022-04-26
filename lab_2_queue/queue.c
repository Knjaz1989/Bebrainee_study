#include <stdio.h>
#include "structures.h"
#include "functions.h"

int main() {
    DB *db = create_db();
    add_node(db, "Hello");
    add_a_certain_amount_into_db(db, 10);
    print_db(db);
    get_node(db);
    print_db(db);
    return 0;
}
