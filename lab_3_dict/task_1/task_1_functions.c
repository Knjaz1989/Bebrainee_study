#include "prototype.h"
#include "task_1_prototype.h"


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
