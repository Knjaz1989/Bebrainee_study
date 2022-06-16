#include "functions.c"


int main() {
    User *p1 = user_constructor("Ivan", "Ivanov", 20, "Russia");
    p1->dump->yaml(p1);
    p1->dump->json(p1);
    p1->dump->xml(p1);
    return 0;
}
