#include "functions.c"


int main() {
    User *p1 = user_cons("Ivan", "Ivanov", 20, "Russia");
    p1->export_data("yaml", p1);
    return 0;
}
