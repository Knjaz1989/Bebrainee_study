#include "header.h"


void export_data(char *type, User *this) {
    if (strcmp(type, "xml") == 0) {
        FILE *file = fopen("data.xml", "w");

        fclose(file);
    }
    else if (strcmp(type, "json") == 0) {
        FILE *file = fopen("data.json", "w");

        fclose(file);
    }
    else if (strcmp(type, "yaml") == 0 || strcmp(type, "yml") == 0) {
        FILE *file = fopen("data.yaml", "w");
        fprintf(file, "- name: %s\n", this->name);
        fprintf(file, "  surname: %s\n", this->surname);
        fprintf(file, "  age: %d\n", this->age);
        fprintf(file, "  address: %s\n", this->address);
        fclose(file);
    }
}

User *user_cons(char *name, char *surname, int age, char *address) {
    User *new_user = (User *)malloc(sizeof(User));
    new_user->name = name;
    new_user->surname = surname;
    new_user->age = age;
    new_user->address = address;
    new_user->export_data = &export_data;
    return new_user;
}
