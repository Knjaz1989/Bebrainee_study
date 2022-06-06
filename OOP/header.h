#ifndef OOP_HEADER_H
#define OOP_HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlohmann/json.hpp"


typedef struct User {
    char *name;
    char *surname;
    int age;
    char *address;
    void (*export_data)(char *type, struct User *this);
} User;

void export_data(char *type, User *this);
User *user_cons(char *name, char *surname, int age, char *address);


#endif //OOP_HEADER_H
