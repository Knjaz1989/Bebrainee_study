#include "prototype.h"
#include "task_2_prototype.h"


int main() {
    int table_size = 800;
    HashTable *Table = create_table(table_size);
    insert_buses(Table);
    write_buses_to_file(Table, "buses.txt");
    printf("%d\n", count_buses_from_file("buses.txt"));
    return 0;
}
