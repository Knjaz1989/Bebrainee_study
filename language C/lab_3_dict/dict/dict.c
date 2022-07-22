#include "prototype.h"


//-----------------------Main------------------------------


int main() {
    int table_size = 1000;
    HashTable *Table = create_table(table_size);
    insert(Table, "good", "tomato"); //get index 25, add item
    insert(Table, "good", "potato"); //get index 25, change value
    insert(Table, "gone", "cucumber"); //get index 25, solve collision
    printf("%s", get_value(Table, "key")); //get error
    printf("%s", get_value(Table, "gone"));
    delete(Table, "key"); //get error
    delete(Table, "gone");
    delete_table(Table);
    return 0;
}
