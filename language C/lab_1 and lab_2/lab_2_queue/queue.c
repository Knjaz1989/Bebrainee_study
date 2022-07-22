#include "prototypes.h"
#include "queue_prototypes.h"

int main() {
    DB *db = create_db();
    push(db, "Hello");
    push(db, "World");
    printf("[%s]\n", pop(db));
    insert_into_random_place(db, 100000);
    print_db(db);
    printf("%d\n", get_db_size(db));
    printf("%s\n", pop_random_value(db));
    return 0;
}
