#include "prototype.h"
#include "task_1_prototype.h"


int main() {
    int table_size = 100;
    HashTable *Table = create_table(table_size);
    printf("%s\n", _random_auto_number());
    generate_random_numbers(Table, table_size);
    check_using(Table, 10);
    return 0;
}
