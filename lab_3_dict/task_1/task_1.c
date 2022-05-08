#include <stdlib.h>
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

char *_random_auto_number() {
    char letters[] = "ACEHKMOPTXY";
    char numbers[] = "0123456789";
    char *auto_number = malloc(7);
    for(int i = 0; i < 6; i++) {
        if (0 < i && i < 4) {
            auto_number[i] = numbers[rand() % (sizeof(numbers) - 1)];
            continue;
        }
        auto_number[i] = letters[rand() % (sizeof(letters) - 1)];
    }
    return auto_number;
}

void generate_random_numbers(HashTable *Table, int count) {
    for (int i = 0; i < count; i++) {
        char *auto_number = _random_auto_number();
        int index = hash_function(auto_number, Table->size);
        insert(Table, auto_number, "");
    }
}

void check_using(HashTable *Table, int count) {
    for (int i = 0; i < count; i++) {
        char *auto_number = _random_auto_number();
        int index = hash_function(auto_number, Table->size);
        if (Table->items[index] != NULL) {
            printf("Collision in index %d\n", index);
            continue;
        }
        insert(Table, auto_number, "");
    }
}
