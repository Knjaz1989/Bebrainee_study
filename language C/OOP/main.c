#include "functions.c"


int main() {
    User **users = list_of_users();

    Type **type_list = type_list_constructor();

    int i = 0;
    while (users[i]) {
        for (int j = 0; j < 3; j++) {
            type_list[j]->dump(users[i]);
        }
        i++;
    }
    return 0;
}
